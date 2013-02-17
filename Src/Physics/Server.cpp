/**
@file Server.cpp

Contiene la implementación del servidor de física. 

@see Physics::CServer

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "Server.h"
#include "Conversions.h"
#include "ErrorManager.h"
#include "CollisionManager.h"
#include "Logic/Entity/Components/Physics.h"

#include <assert.h>
#include <algorithm>

#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h>
#include <extensions\PxVisualDebuggerExt.h> 
#include <RepX\RepX.h>

using namespace Physics;
using namespace Logic;
using namespace physx;


// Única instancia del servidor
CServer *CServer::_instance = NULL;

//--------------------------------------------------------

CServer::CServer() : _cudaContextManager(NULL), _scene(NULL)
{
	// Crear gestor de errores
	_errorManager = new CErrorManager();

	// Crear gestor de memoria
	_allocator = new PxDefaultAllocator();

	// Crear gestor de colisiones
	_collisionManager = new CCollisionManager();

	// Crear PxFoundation. Es necesario para instanciar el resto de objetos de PhysX
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorManager);
	assert(_foundation && "Error en PxCreateFoundation");

	// Crear PxProfileZoneManager. Es necesario para habitiar algunas opciones de 
	// profiling de rendimiento en el PhysX Visual Debugger 
	_profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(_foundation);
	assert(_profileZoneManager && "Error en PxProfileZoneManager::createProfileZoneManage");

	// Crear PxPhysics. Es el punto de entrada al SDK de PhysX
	PxTolerancesScale toleranceScale;
	bool recordMemoryAllocations = true;
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, toleranceScale, 
		                       recordMemoryAllocations, _profileZoneManager);
	assert(_physics && "Error en PxCreatePhysics");

	// Crear CudaContextManager. Permite aprovechar la GPU para hacer parte de la simulación física.
	// Se utiliza posteriormente al crear la escena física.
	// Sólo Windows
#ifdef PX_WINDOWS

	pxtask::CudaContextManagerDesc cudaContextManagerDesc;
	_cudaContextManager = pxtask::createCudaContextManager(*_foundation, cudaContextManagerDesc, 
		                                                   _profileZoneManager);
	if( _cudaContextManager )
	{
		if( !_cudaContextManager->contextIsValid() )
		{
			_cudaContextManager->release();
			_cudaContextManager = NULL;
		}
	}

#endif

	// Crear PxControllerManager. Es necesario para crear character controllers
	_controllerManager = PxCreateControllerManager(*_foundation);

	// Inicializar el módulo PxCooking. Es necesario para cocinar mallas y para 
	// deserializar actores a partir de ficheros RepX
	PxCookingParams params;
	_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *_foundation, params);

	// Crear el material que se usará por defecto
	float staticFriction = 0.5f;
	float dynamicFriction = 0.5f;
	float restitution = 0.1f;
	_defaultMaterial = _physics->createMaterial(staticFriction, dynamicFriction, restitution);

	// Intentar conectar con PhysX Visual Debugger (PVD)
	_pvdConnection = NULL;

	// Sólo en modo DEBUG
#ifdef _DEBUG
	debugger::comm::PvdConnectionManager *pvdConnectionManager = _physics->getPvdConnectionManager();
	assert(pvdConnectionManager && "Error en PxPhysics::getPvdConnectionManager");

	// Configurar máquina, puerto y tiempo de espera (en millisegundos)
	const char *ip = "127.0.0.1";
	int port = 5425;						
	unsigned int timeout = 100;				

	// Configurar qué información queremos mandar al PVD (debug, profile, memory)
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

	// Intentar establecer la conexión
	_pvdConnection = PxVisualDebuggerExt::createConnection(pvdConnectionManager, ip, port, timeout, connectionFlags);

#endif
} 

//--------------------------------------------------------

CServer::~CServer() 
{
	// Destruir objetos en orden inverso a como fueron creados
	if (_pvdConnection) {
		_pvdConnection->release();
		_pvdConnection = NULL;
	}

	if (_defaultMaterial) {
		_defaultMaterial->release();
		_defaultMaterial = NULL;
	}

	if (_cooking) {
		_cooking->release();
		_cooking = NULL;
	}

	if (_controllerManager) {
		_controllerManager->release();
		_controllerManager = NULL;
	}

	if (_cudaContextManager) {
		_cudaContextManager->release();
		_cudaContextManager = NULL;
	}

	if (_physics) {
		_physics->release();
		_physics = NULL;
	}

	if (_profileZoneManager) { 
		_profileZoneManager->release();
		_profileZoneManager = NULL;
	}

	if (_foundation) {
		_foundation->release();
		_foundation = NULL;
	}
	
	if (_collisionManager) {
		delete _collisionManager;
		_collisionManager = NULL;
	}

	if (_allocator) {
		delete _allocator;
		_allocator = NULL;
	}

	if (_errorManager) {
		delete _errorManager;
		_errorManager = NULL;
	}
} 

//--------------------------------------------------------

bool CServer::Init() 
{
	if (!_instance) {
		_instance = new CServer();
	}

	return true;
} 

//--------------------------------------------------------

void CServer::Release()
{
	if(_instance) {
		delete _instance;
		_instance = NULL;
	}
} 

//--------------------------------------------------------

void CServer::createScene ()
{
	assert(_instance);
	
	// Crear el descriptor de la escena
	PxSceneDesc sceneDesc(_physics->getTolerancesScale());

	// Establecer la gravedad en el eje Y
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	// Establecer el gestor de colisiones
	sceneDesc.simulationEventCallback = _collisionManager;

	// Establecer un gestor de tareas por CPU
	if (!sceneDesc.cpuDispatcher) {
		int mNbThreads = 1; // hilos de ejecución
		_cpuDispatcher  = PxDefaultCpuDispatcherCreate(mNbThreads);
		assert (_cpuDispatcher && "Error en PxDefaultCpuDispatcherCreate");

		sceneDesc.cpuDispatcher = _cpuDispatcher;
	}

	// Establecer el shader que controla las colisiones entre entidades.
	// Usamos un shader que emula la gestión de grupos de PhysX 2.8
	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	// Intentar establecer un gestor de tareas por GPU 
	// Sólo Windows
#ifdef PX_WINDOWS
	if (!sceneDesc.gpuDispatcher && _cudaContextManager)
	{
		sceneDesc.gpuDispatcher = _cudaContextManager->getGpuDispatcher();
	}
#endif

	// Crear la escena física
	_scene = _physics->createScene(sceneDesc);
	assert(_scene && "Error en PxPhysics::createScene");
}

//--------------------------------------------------------

void CServer::destroyScene ()
{
	assert(_instance);

	if (_scene) {
		_scene->release();
		_scene = NULL;
	}
}

//--------------------------------------------------------

bool CServer::tick(unsigned int msecs) 
{
	assert(_scene);

	// Empezar la simulación física. Actualmente usamos intervalos de tiempo variables,
	// debemos tener cuidado porque PhysX puede no comportarse bien si el tiempo 
	// transcurrido es demasiado grande.
	_scene->simulate(msecs / 1000.0f);

	// Esperamos a que la simulación física termine. En principio podríamos utilizar
	// este intervalo de tiempo para hacer algo más útil. Existe una versión de este
	// método no bloqueante.
	return _scene->fetchResults(true);
} 

//--------------------------------------------------------

PxRigidStatic* CServer::createPlane(const Vector3 &point, const Vector3 &normal, int group, 
	                                const IPhysics *component)
{
	assert(_scene);

	// Crear un plano estático
	PxPlane plane(Vector3ToPxVec3(point), Vector3ToPxVec3(normal));
	PxMaterial *material = _defaultMaterial;
	PxRigidStatic *actor = PxCreatePlane(*_physics, plane, *material);
	
	// Anotar el componente lógico asociado a la entidad física
	actor->userData = (void *) component;

	// Establecer el grupo de colisión
	PxSetGroup(*actor, group);
	
	// Añadir el actor a la escena
	_scene->addActor(*actor);

	return actor;
}

//--------------------------------------------------------

PxRigidStatic* CServer::createStaticBox(const Vector3 &position, const Vector3 &dimensions, bool trigger, 
	                                    int group, const IPhysics *component)
{
	assert(_scene);

	// Nota: PhysX coloca el sistema de coordenadas local en el centro de la caja, mientras
	// que la lógica asume que el origen del sistema de coordenadas está en el centro de la 
	// cara inferior. Para unificar necesitamos realizar una traslación en el eje Y.
	// Afortunadamente, el descriptor que se usa para crear el actor permite definir esta 
	// transformación local, por lo que la conversión entre sistemas de coordenadas es transparente. 
	
	// Crear un cubo estático
	PxTransform pose(Vector3ToPxVec3(position));
	PxBoxGeometry geom(Vector3ToPxVec3(dimensions));
	PxMaterial *material = _defaultMaterial;
	PxTransform localPose(PxVec3(0, dimensions.y, 0)); // Transformación de coordenadas lógicas a coodenadas de PhysX
	PxRigidStatic *actor = PxCreateStatic(*_physics, pose, geom, *material, localPose);
	
	// Transformarlo en trigger si es necesario
	if (trigger) {
		PxShape *shape;
		actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// Anotar el componente lógico asociado a la entidad física
	actor->userData = (void *) component;

	// Establecer el grupo de colisión
	PxSetGroup(*actor, group);

	// Añadir el actor a la escena
	_scene->addActor(*actor);
	
	return actor;
}

//--------------------------------------------------------

PxRigidDynamic* CServer::createDynamicBox(const Vector3 &position, const Vector3 &dimensions, 
	                                      float mass, bool kinematic, bool trigger, int group, 
										  const IPhysics *component)
{
	assert(_scene);

	// Nota: PhysX coloca el sistema de coordenadas local en el centro de la caja, mientras
	// que la lógica asume que el origen del sistema de coordenadas está en el centro de la 
	// cara inferior. Para unificar necesitamos realizar una traslación en el eje Y.
	// Afortunadamente, el descriptor que se usa para crear el actor permite definir esta 
	// transformación local, por lo que la conversión entre sistemas de coordenadas es transparente. 

	// Crear un cubo dinámico
	PxTransform pose(Vector3ToPxVec3(position));
	PxBoxGeometry geom(Vector3ToPxVec3(dimensions));
	PxMaterial *material = _defaultMaterial;
	float density = mass / (dimensions.x * dimensions.y * dimensions.z);
	PxTransform localPose(PxVec3(0, dimensions.y, 0)); // Transformación de coordenadas lógicas a coodenadas de PhysX

	// Crear cubo dinámico o cinemático
	PxRigidDynamic *actor;
	if (kinematic)
		actor = PxCreateKinematic(*_physics, pose, geom, *material, density, localPose);
	else
		actor = PxCreateDynamic(*_physics, pose, geom, *material, density, localPose);
	
	// Transformarlo en trigger si es necesario
	if (trigger) {
		PxShape *shape;
		actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// Anotar el componente lógico asociado a la entidad física
	actor->userData = (void *) component;

	// Establecer el grupo de colisión
	PxSetGroup(*actor, group);

	// Añadir el actor a la escena
	_scene->addActor(*actor);

	return actor;
}

//--------------------------------------------------------

PxRigidActor* CServer::createFromFile(const std::string &file, int group, const IPhysics *component)
{
	assert(_scene);

	// Preparar parámetros para deserializar
	PxDefaultFileInputData data(file.c_str());
	PxCollection* bufferCollection = _physics->createCollection();
	PxCollection* sceneCollection = _physics->createCollection();
	PxStringTable* stringTable = NULL; 
	PxUserReferences* externalRefs = NULL; 
	PxUserReferences* userRefs = NULL; 

	// Deserializar a partir del fichero RepX
	repx::deserializeFromRepX(data, *_physics, *_cooking, stringTable, externalRefs, 
		                      *bufferCollection, *sceneCollection, userRefs);
	
	// Añadir entidades físicas a la escena
	_physics->addCollection(*sceneCollection, *_scene); 
	
	// Buscar una entidad de tipo PxRigidActor. Asumimos que hay exactamente 1 en el fichero.
	PxRigidActor *actor = NULL;
	for (unsigned int i=0; (i<sceneCollection->getNbObjects()) && !actor; i++) {
		PxSerializable *p = sceneCollection->getObject(i);
		actor = p->is<PxRigidActor>();
		
	}
	assert(actor);
	
	// Anotar el componente lógico asociado a la entidad física
	actor->userData = (void *) component;

	// Establecer el grupo de colisión
	PxSetGroup(*actor, group);

	// Liberar recursos
	bufferCollection->release();
	sceneCollection->release();

	return actor;
}

//--------------------------------------------------------

void CServer::destroyActor(physx::PxActor *actor)
{
	assert(_scene);

	// Eliminar el actor de la escena
	_scene->removeActor(*actor);

	// Liberar recursos
	actor->release();
}

//--------------------------------------------------------

Matrix4 CServer::getActorTransform(const PxRigidActor *actor)
{
	assert(actor);

	// Devolver la posición y orientación en coordenadas lógicas
	return PxTransformToMatrix4(actor->getGlobalPose());
}

//--------------------------------------------------------

void CServer::moveKinematicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform)
{
	assert(actor);
	assert(isKinematic(actor));

	// Mover el actor tras transformar el destino a coordenadas lógicas
	actor->setKinematicTarget(Matrix4ToPxTransform(transform));
}

//--------------------------------------------------------

void CServer::moveKinematicActor(physx::PxRigidDynamic *actor, const Vector3 &displ)
{
	assert(actor);
	assert(isKinematic(actor));

	// Desplazar el actor
	PxTransform transform = actor->getGlobalPose();
	transform.p += Vector3ToPxVec3(displ);
	actor->setKinematicTarget(transform);
}

//--------------------------------------------------------

bool CServer::isKinematic(const PxRigidDynamic *actor)
{
	assert(actor);

	return actor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC;
}

//--------------------------------------------------------

PxCapsuleController* CServer::createCapsuleController(const Vector3 &position, float radius, 
	                                                  float height, const CPhysicController *component)
{
	assert(_scene);

	// Nota: PhysX coloca el sistema de coordenadas local en el centro de la cápsula, mientras
	// que la lógica asume que el origen del sistema de coordenadas está en los piés del 
	// jugador. Para unificar necesitamos realizar una traslación en el eje Y.
	// Desafortunadamente, el descriptor que se usa para crear los controllers no permite
	// definir esta transformación local (que sí permite al crear un actor), por lo que
	// tendremos que realizar la traslación nosotros cada vez. 

	// Transformación entre el sistema de coordenadas lógico y el de PhysX
	float offsetY = height / 2.0f + radius;
	PxVec3 pos = Vector3ToPxVec3(position + Vector3(0, offsetY, 0));
	
	// Crear descriptor del controller
	PxCapsuleControllerDesc desc;
	desc.position = PxExtendedVec3(pos.x, pos.y, pos.z);
	desc.height = height;
	desc.radius = radius;
	desc.material = _defaultMaterial;
	desc.climbingMode = PxCapsuleClimbingMode::eEASY; 
	//desc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	//desc.slopeLimit = 0.707f;
	desc.callback = _collisionManager;   // Establecer gestor de colisiones
	desc.userData = (void *) component;  // Anotar el componente lógico asociado al controller
	
	PxCapsuleController *controller = (PxCapsuleController *)
		 _controllerManager->createController(*_physics, _scene, desc);
	
	// Anotar el componente lógico asociado al actor dentro del controller (No es automático)
	controller->getActor()->userData = (void *) component;

	return controller;
}

//--------------------------------------------------------

unsigned CServer::moveController(PxController *controller, const Vector3 &movement, unsigned int msecs)
{
	assert(_scene);

	// Mover el character controller y devolver los flags de colisión
	PxVec3 disp = Vector3ToPxVec3(movement);
	float minDist = 0.01f;
	float elapsedTime = msecs / 1000.0f;
	PxControllerFilters filters;
	PxObstacleContext *obstacles = NULL;
	return controller->move(disp, minDist, elapsedTime, filters, obstacles);
}

//--------------------------------------------------------

Vector3 CServer::getControllerPosition(const PxCapsuleController *controller)
{
	assert(_scene);

	// Antes de devolver la posición del controller debemos transformar entre el 
	// sistema de coordenadas de PhysX y el de la lógica
	float offsetY = controller->getHeight() / 2.0f + controller->getRadius();
	Vector3 pos = PxExtendedVec3ToVector3(controller->getPosition());
	return pos - Vector3(0, offsetY, 0);
}

//--------------------------------------------------------

void CServer::setGroupCollisions(int group1, int group2, bool enable)
{
	// Activar / desactivar colisiones entre grupos
	PxSetGroupCollisionFlag(group1, group2, enable);
}

//--------------------------------------------------------

Logic::CEntity* CServer::raycastClosest (const Ray& ray, float maxDist) const
{
	assert(_scene);

	// Establecer parámetros del rayo
	PxVec3 origin = Vector3ToPxVec3(ray.getOrigin());      // origen     
	PxVec3 unitDir = Vector3ToPxVec3(ray.getDirection());  // dirección normalizada   
	PxReal maxDistance = maxDist;                          // distancia máxima
	PxRaycastHit hit;                 

	// Información que queremos recuperar de la posible colisión
	//const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;
	const PxSceneQueryFlags outputFlags;

	// Lanzar el rayo
	bool intersection = _scene->raycastSingle(origin, unitDir, maxDistance, outputFlags, hit);

	// IMPORTANTE: aunque se haya llamado al método move de los controllers y al consultar su posición
	// esta aparezca actualizada, sus actores asociados no se habrán desplazado aún. La consecuencia
	// es que se pueden recuperar colisiones inesperadas.

	if (intersection) {
		// Devolver entidad lógica asociada a la entidad física impactada
		IPhysics *component = (IPhysics *) hit.shape->getActor().userData;
		return component->getEntity();
	} else {
		return NULL;
	}
}

//--------------------------------------------------------

Logic::CEntity* CServer::raycastClosest(const Ray& ray, float maxDist, int group) const 
{
	assert(_scene);

	// Establecer parámetros del rayo
	PxVec3 origin = Vector3ToPxVec3(ray.getOrigin());      // origen     
	PxVec3 unitDir = Vector3ToPxVec3(ray.getDirection());  // dirección normalizada   
	PxReal maxDistance = maxDist;                          // distancia máxima
	PxRaycastHit hit;                 
	const PxSceneQueryFlags outputFlags;				   // Info que queremos recuperar	

	// Lanzar el rayo
	PxRaycastHit hits[64];
	bool blockingHit;
	PxI32 nHits = _scene->raycastMultiple(origin, unitDir, maxDistance, outputFlags, hits, 64, blockingHit); 
	
	// Buscar un actot que pertenezca al grupo de colisión indicado
	for (int i=0; i<nHits; i++) {
		PxRigidActor *actor = &hits[i].shape->getActor();
		if (PxGetGroup(*actor) == group) {
			IPhysics *component = (IPhysics *) actor->userData;
			if (component) {
				return component->getEntity();
			}
		}
	}

	return NULL;

	// Nota: seguro que se puede hacer de manera mucho más eficiente usando los filtros
	// de PhysX.
}

//--------------------------------------------------------