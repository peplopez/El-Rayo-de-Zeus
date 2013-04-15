#include "SightSensor.h"

#include "PerceptionEntity.h"
#include "PerceptionSignal.h"

#include "BaseSubsystems/Math.h"
#include "Physics/Server.h"

#include "Logic/Entity/Entity.h"

namespace AI 
{

CSightSensor::~CSightSensor(void)
{
}

/**
 * Realiza todas las comprobaciones necesarias para averiguar si la señal recibida
 * se encuentra dentro del cono de visión.
 *
 * Las comprobaciones se realizan de más "barata" a más "cara"
 * 1. Comprueba si el sensor y la señal están activos
 * 2. Comprueba si el tipo de la señal se corresponde con el tipo que percibe el sensor.
 * 3. Comprueba si la intensidad de la señal está por encima del threshold del sensor.
 * 4. Comprueba la distancia entre la señal y el sensor. Se realiza en 2 pasos:
 * 4.1. Primero se comprueba si la distancia en cada dimensión es mayor que la distancia máxima.
 * 4.2. A continuación se comprueba si la distancia euclídea es mayor que la distancia máxima.
 * 5. Comprobamos si la señal se encuentra dentro del ángulo de amplitud del cono de visión.
 * 6. Comprobamos si no existe ningún objeto físico entre el sensor y la señal. Para eso usamos un rayo físico.
 *
 * En el momento en que alguna de estas comprobaciones falle se detiene el proceso y se devuelve
 * NULL,  lo que significa que el sensor no es capaz de detectar la señal. Por otro lado, si todas se
 * superan con éxito se devuelve una notificación.
 *
 * @param perceptible Señal cuya percepción queremos comprobar
 * @param time Instante en el que se realiza la comprobación de percepción
 * @return NULL si no se ha percibido la señal. Una instancia de CNotification en caso contrario.
 */
CNotification* CSightSensor::perceives(const CPerceptionSignal * signal, unsigned int time) 
{

	// Realizamos las comprobaciones
	// Si alguna de ellas falla tendremos que devolver NULL para indicar que no ha habido percepción
	// Si todas tienen éxito devolveremos una instancia de CNotification

	// Comprobaciones: 
	// 1. Comprueba si el sensor y la señal están activos 
	// (método isActive de la señal - se recibe como parámetro- y del sensor)
	if (!(this->isActive()) || !(signal->isActive()))
	{
		//cout << "Fallo: inactivo" << endl;
		return NULL;
	}
	// 2. Comprueba si el tipo de la señal se corresponde con el tipo que percibe el sensor
	// (método getType)
	if (this->getType() != signal->getType())
	{
		//cout << "Fallo: tipo" << endl;
		return NULL;
	}
	// 3. Comprueba si la intensidad de la señal está por encima del threshold del sensor.
	// La intensidad que percibimos, F, es el resultado de usar una función de atenuación:
	// f(F0, dist) = F	donde F0 es la intensidad inicial y dist la distancia.
	// En nuestro caso vamos a simplificar y usaremos una función constante:
	// f(F0, dist) = F0
	// Es decir, que sólo tendremos que comprobar que el threshold sea mayor que la intensidad
	if (signal->getIntensity() < this->getThreshold())
	{
		//cout << "Fallo: intensidad" << endl;
		return NULL;
	}
	// 4. Comprueba la distancia entre la señal y el sensor. Se realiza en 2 pasos:
	// 4.1. Primero se comprueba si la distancia en cada dimensión es mayor que la distancia máxima.
	// Sacamos la matriz de transformación de la entidad de percepción (getTransform) 
	// y con ella podemos obtener la posición del sensor (getTrans)
	// Hacemos lo mismo con la señal. La diferencia entre estos vectores es el vector de distancia
	// Comparamos cada una de sus dimensiones con la distancia máxima
	Matrix4 transform = this->getPerceptionEntity()->getTransform();
	Vector3 position = transform.getTrans();
	Vector3 signalPosition = signal->getPerceptionEntity()->getTransform().getTrans();
	Vector3 distance = signalPosition - position;
	if (abs(distance.x) > _maxDistance || abs(distance.y) > _maxDistance || abs(distance.z) > _maxDistance)
	{
		//cout << "Fallo: distancia 1" << endl;
		return NULL;
	}
	// 4.2. A continuación se comprueba si la distancia euclídea es mayor que la distancia máxima.
	// Usamos el vector distancia que hemos calculado antes
	float distMagnitude = distance.length();
	if (distMagnitude > _maxDistance)
	{
		//cout << "Fallo: distancia 2" << endl;
		return NULL;
	}
	// Comprobamos el cono de visión


	// 5. Comprobamos si la señal se encuentra dentro del ángulo de amplitud del cono de visión.
	// Tenemos que calcular el ángulo que se forma entre la dirección hacia la que apunta la entidad (su orientación) y
	// la dirección en la que se encuentra el sensor (con respecto a la entidad)
	// Sacamos la orientación de la entidad (getYaw nos da el ángulo de la entidad con respecto al eje Z)
	// Sacamos la orientación de la señal = orientación del vector distancia
	// Y sacamos la diferencia 
	// Nos aseguramos de que el ángulo es menor que PI (nos tenemos que quedar con la parte pequeña de la diferencia)
	// Sacamos la orientación de la entidad 
	// (yaw nos da el ángulo de la entidad con respecto al eje Z)
	float yaw = Math::getYaw(transform);
	// Hacemos lo mismo con el vector distancia 
	// (que va desde el sensor hasta la señal)
	Ogre::Radian angle = Ogre::Math::ATan2(-distance.x, -distance.z);
	float signalAngle = angle.valueRadians();
	// El ángulo de la señal con respecto a la entidad es signalAngle
	float finalAngle = abs(yaw - signalAngle);
	// Nos aseguramos de que el ángulo es menor que PI
	if (finalAngle > Ogre::Math::PI)
		finalAngle = Ogre::Math::TWO_PI - finalAngle;
	if (finalAngle > _alpha)
	{
		//cout << "Fallo: cono" << endl;
		return NULL;
	}
	
	// 6. Comprobamos si no existe ningún objeto físico entre el sensor y la señal. Para eso usamos un rayo físico.
	// Necesitamos
	// · la posición de origen del rayo = posición del sensor
	// · la dirección (normalizada) = vector distance normalizado
	// · la distancia máxima = magnitud del vector distance
	// Con estos parámetros creamos una instancia de Ogre::Ray
	// Y lo usamos con raycastClosest. 
	// Si el rayo colisiona con algún objeto físico es que hay algún tipo de pared entre el sensor 
	// y la señal, por lo que la entidad que ha emitido la señal no se puede percibir.
	// Si hay algún obstáculo ==> raycastClosest nos devuelve la referencia ==> return NULL
	Vector3 origin, direction;
	float maxDistance;

	origin = position;
	direction = distance / distMagnitude;
	maxDistance = distMagnitude;

	// Dependiendo de la configuración de la detección de colisiones,
	// si lanzáramos el rayo con estos parámetros, chocaría contra la propia
	// cápsula del lanzador, por lo que tendríamos que desplazarlo un poco (lo que mide su radio)
	// En concreto
	// · la nueva posición de origen se desplaza a lo largo de la dirección del vector
	//   lo que mide el radio: o' = o + r * <dirección normalizada>
	// · la dirección sigue siendo la misma
	// · la nueva distancia máxima es la distancia anterior recortándole al principio y al 
	//   final los radios de las entidades del sensor y la señal.
	//direction = distance / distMagnitude;
	//float sensorRadius = _pEntity->getRadius();
	//origin = position + sensorRadius * direction;
	//float signalRadius = signal->getPerceptionEntity()->getRadius();
	//maxDistance = distMagnitude - sensorRadius - signalRadius;

	Ray ray = Ray(position, direction);
	// TODO Si accedemos a la entidad se rompe la independencia entre esta clase y la lógica
	Logic::CEntity* collisionedEntity =NULL;//PEP HACK// Physics::CServer::getSingletonPtr()->raycastClosest(ray, maxDistance);
	// Si el rayo colisiona con algún objeto físico es que hay algún tipo de pared entre el sensor 
	// y la señal, por lo que la entidad que ha emitido la señal no se puede percibir.
	// TODO habría que modificar la física para permitir desactivar las colisiones del rayo con los triggers. En la versión actual los triggers contarían como un objeto opaco.
	if (collisionedEntity != NULL)
	{
		//cout << "Fallo: rayo" << endl;
		return NULL;
	}

	// Si todos los chequeos han tenido éxito tenemos que devolver una nueva instancia de CNotification
	// Los parámetros que necesita son:
	// · El instante en el que hay que entregarla (ahora mismo ==> time)
	// · El sensor que lo ha percibido (this)
	// · La entidad de percepción que ha generado la señal
	// Devolvemos la notificación para que se envíe a la entidad

	return new CNotification(time, this, signal->getPerceptionEntity(), Vector3::ZERO);
}


} // namespace AI 
