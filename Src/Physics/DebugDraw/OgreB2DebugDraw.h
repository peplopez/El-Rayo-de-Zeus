#ifndef _OGRE_B2_DEBUGDRAW_H
#define _OGRE_B2_DEBUGDRAW_H

#include <Ogre.h>
#include <Box2D/Box2D.h>


struct Vertex
{
	float32	x;
	float32	y;
	float32	z;
	
	float32	r;
	float32	g;
	float32	b;
	float32	a;
};


class OgreB2DebugDraw : public b2Draw
{
public:
    OgreB2DebugDraw(Ogre::SceneManager* scene, const char* material, float fillAlpha = 0.5f, Ogre::uint8 renderQueueGroup = Ogre::RENDER_QUEUE_OVERLAY);
    ~OgreB2DebugDraw();

    /// Method to be called each frame to clear out all of last frame's shapes.
    void clear();

	void Render();
	void RenderLines();
	void RenderTriangles();

	void setAutoTracking(Ogre::SceneNode* target);

    // Methods for accessing rendering options.
    inline const char* getMaterialName() {
        return m_material.c_str();
    }
    inline float getFillAlpha() {
        return m_fillAlpha;
    }
    inline Ogre::uint8 getRenderQueueGroup() {
        return m_shapes->getRenderQueueGroup();
    }
    // Methods for setting rendering options.
    inline void setMaterialName(const char* material) {
        m_material = material;
    }
    inline void setFillAlpha(float fillAlpha) {
        m_fillAlpha = fillAlpha;
    }
    inline void setRenderQueueGroup(Ogre::uint8 renderQueueGroup) {
        m_shapes->setRenderQueueGroup(renderQueueGroup);
    }

    // Draw methods are called during b2World::Step on the b2DebugDraw object currently
    // registered with the b2World. The result is a ManualObject with all the debug data will
    // be added to the scene in the RENDER_QUEUE_OVERLAY group.
    // Remember that they are public so you can call them if you like outside of b2World::Step.

    /// Method called by the b2World for empty polygon outlines (e.g. bounding boxes)
   void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    /// Method called by the b2World for filled polygons; used for most shapes.
   void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    /// Method called by the b2World for empty circle outlines.
   void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

    /// Method called by the b2World for filled circles; used for cicular shapes.
   void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

    /// Method called by the b2World for line segments (e.g. joints).
   void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

    /// Method called by the b2World for XForms.
   void DrawTransform(const b2Transform& xf);

   void enable();
   void disable();

private:
    /// ManualObject which holds all the shapes each frame
    Ogre::ManualObject  *m_shapes;
    /// Pointer to the scene to which the shapes are added
    Ogre::SceneManager  *m_scene;

	Ogre::SceneNode *_debugNode;

    /// Material to use for rendering the shapes
    Ogre::String m_material;

	typedef std::vector<Vertex> TDebugRenderable;


	TDebugRenderable _lineList;
	TDebugRenderable _triangleList;


    /// Alpha value to use for filling in shapes;
    /// if m_fillAlpha <= 0, they will not be filled, saving system resources.
    float m_fillAlpha;

    /// Helper method for writing polygon vertex data to a manual object.
    /// @warning Must be called between ManualObject::begin and ManualObject::end
    void BuildPolygon(const b2Vec2* vertices, int32 vertexCount, 
					  const b2Color& color, TDebugRenderable &vertexSection, float alpha = 1.0f);

	void BuildSolidPolygon(const b2Vec2* vertices, int32 vertexCount, 
					  const b2Color& color, TDebugRenderable &v, float alpha = 1.0f);

    /// Helper method for writing circle vertex data to a manual object.
    /// @warning Must be called between ManualObject::begin and ManualObject::end
    void BuildCircle(const b2Vec2& center, float32 radius, 
				     const b2Color& color, TDebugRenderable &v, float alpha = 1.0f);

	void BuildSolidCircle(const b2Vec2& center, float32 radius, 
				     const b2Color& color, TDebugRenderable &v, float alpha = 1.0f);

    /// Helper method for writing line segment vertex data to a manual object.
    /// @warning Must be called between ManualObject::begin and ManualObject::end
    void BuildSegment(const b2Vec2& p1, const b2Vec2& p2, 
					  const b2Color& color, TDebugRenderable &v, float alpha = 1.0f);
};

#endif //_OGRE_B2_DEBUGDRAW_H