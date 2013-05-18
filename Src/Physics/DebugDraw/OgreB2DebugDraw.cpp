#include "OgreB2DebugDraw.h"
#include <OgreVector3.h>

OgreB2DebugDraw::OgreB2DebugDraw(Ogre::SceneManager* scene, const char* material, float fillAlpha, Ogre::uint8 renderQueueGroup) :
    b2Draw(),
    m_scene(scene),
    m_material(material),
    m_fillAlpha(fillAlpha),
    m_shapes()
{
    // Create the manual object.
    m_shapes = m_scene->createManualObject("OgreB2DebugDrawShapes");


    _debugNode = m_scene->getRootSceneNode()->createChildSceneNode("OgreB2DebugDrawNode");
	_debugNode->attachObject(m_shapes);
	_debugNode->yaw(Ogre::Degree(90.0f));

	
    // Make it dynamic since we will be rewriting the data many times each frame.
    m_shapes->setDynamic(true);
	

    // Add it to the given render queue group to control render order.
    m_shapes->setRenderQueueGroup(renderQueueGroup);

}


OgreB2DebugDraw::~OgreB2DebugDraw()
{
    m_scene->destroyManualObject("OgreB2DebugDrawShapes");
    m_scene->destroySceneNode("OgreB2DebugDrawNode");
}

void OgreB2DebugDraw::setAutoTracking(Ogre::SceneNode* target)
{
	_debugNode->setFixedYawAxis(true);
	_debugNode->setAutoTracking(true, target, Ogre::Vector3::NEGATIVE_UNIT_Z);
}

void OgreB2DebugDraw::clear()
{
	m_shapes->clear();
}

void OgreB2DebugDraw::Render()
{
	m_shapes->begin(m_material, Ogre::RenderOperation::OT_LINE_LIST);

	m_shapes->estimateVertexCount(_lineList.size());

	for (int i = 0; i < _lineList.size(); ++i)
	{
		m_shapes->position(_lineList[i].x, _lineList[i].y, _lineList[i].z);
		m_shapes->colour(_lineList[i].r, _lineList[i].g, _lineList[i].b, _lineList[i].a);
	}
	_lineList.clear();
	m_shapes->end();

	
}

void OgreB2DebugDraw::BuildPolygon(const b2Vec2* vertices,
                                             int32 vertexCount, const b2Color& color, TDebugRenderable& vertexSection, float alpha)
{
	// We estimate the vertex count beforehand to optimize allocation.
	// Note that we add one since Box2D does not count overlap vertex.
	//man->estimateVertexCount(vertexCount+1);

	Vertex vertex;
	vertex.r = color.r; vertex.g = color.g; vertex.b = color.b; vertex.a = alpha;
	vertex.z = 0;
	// Place the vertices.
	for (int32 i = 0; i < vertexCount -1 ; ++i)
	{
		//man->position(vertices[i].x, vertices[i].y, 0);
		//man->colour(color.r, color.g, color.b, alpha);
		vertex.x = vertices[i].x; vertex.y = vertices[i].y; 
		vertexSection.push_back(vertex);

		vertex.x = vertices[i+1].x; vertex.y = vertices[i+1].y; 
		vertexSection.push_back(vertex);
	}

	vertexSection.push_back(vertex);
	vertex.x = vertices[0].x; vertex.y = vertices[0].y;
	vertexSection.push_back(vertex);
}

void OgreB2DebugDraw::BuildCircle(const b2Vec2& center, float32 radius,
									const b2Color& color, TDebugRenderable& vertexSection, float alpha)
{
	// circle variables
	const float k_segments = 32.0f;
	const float k_increment = 2.0f * b2_pi / k_segments;
	float theta = 0.0f;

	// We estimate the vertex count beforehand to optimize allocation.
	// Note that we add one since Box2D does not count overlap vertex.
	//man->estimateVertexCount(k_segments+1);

	// Update object data.
	b2Vec2 v1, v2;
	Vertex vertex;
	vertex.r = color.r; vertex.g = color.g; vertex.b = color.b; vertex.a = alpha;
	vertex.z = 0;
	for (int32 i = 0; i < k_segments; ++i)
	{
		v1 = center + radius * b2Vec2(cosf(theta), sinf(theta));
		vertex.x = v1.x; vertex.y = v1.y;
		vertexSection.push_back(vertex);
		theta += k_increment;
		v2 = center + radius * b2Vec2(cosf(theta), sinf(theta));
		vertex.x = v2.x; vertex.y = v2.y;
		vertexSection.push_back(vertex);
	}

}

void OgreB2DebugDraw::BuildSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color, TDebugRenderable& vertexSection, float alpha)
{
    // We estimate the vertex count beforehand to optimize allocation.
    //man->estimateVertexCount(2);

    // Place the first vertex.
	Vertex vertex;
	vertex.r = color.r; vertex.g = color.g; vertex.b = color.b; vertex.a = alpha;
	vertex.z = 0;

	vertex.x = p1.x; vertex.y = p1.y;
    vertexSection.push_back(vertex);

    // Place the second vertex.
    vertex.x = p2.x; vertex.y = p2.y;
    vertexSection.push_back(vertex);
}

void OgreB2DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    // Call helper method to generate vertex data.
    BuildPolygon(vertices, vertexCount, color, _lineList);
}

void OgreB2DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

	// Call helper method to generate vertex data.
	BuildPolygon(vertices, vertexCount, color, _lineList);

	// Only fill if it will be visible.
	if (m_fillAlpha > 0.01f)
	{
		// Call helper method to generate vertex data.
		//BuildPolygon(vertices, vertexCount, color, _triangleFan, m_fillAlpha);

	}
}

void OgreB2DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{

	// Call helper method to generate vertex data.
	BuildCircle(center, radius, color, _lineList);

}

void OgreB2DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{

	// Call helper method to generate vertex data.
	BuildCircle(center, radius, color, _lineList);

	// Only fill if it will be visible.
	if (m_fillAlpha > 0.01f)
	{
		// Call helper method to generate vertex data.
		//BuildCircle(center, radius, color, _triangleFan, m_fillAlpha);

	}
}

void OgreB2DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{

	// Call helper method to generate vertex data.
	BuildSegment(p1, p2, color, _lineList);

}

void OgreB2DebugDraw::DrawTransform(const b2Transform& xf)
{
	// XForm data
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 1.0f;

	// Add a red segment.
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	BuildSegment(p1, p2, b2Color(1,0,0), _lineList);

	// Add a green segment.
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	BuildSegment(p1, p2, b2Color(0,1,0), _lineList);

}