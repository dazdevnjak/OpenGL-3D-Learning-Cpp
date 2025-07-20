#pragma once
#include "LinearMath/btIDebugDraw.h"
#include <glut.h>
#include "LinearMath/btVector3.h"
#include "LinearMath/btScalar.h"
#include "LinearMath/btIDebugDraw.h"


class GLDebugDrawer : public btIDebugDraw
{
    int m_debugMode;

public:
    GLDebugDrawer() : m_debugMode(0) {}
    virtual ~GLDebugDrawer() {}

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
    {
        glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
        glBegin(GL_LINES);
        glVertex3f(from.getX(), from.getY(), from.getZ());
        glVertex3f(to.getX(), to.getY(), to.getZ());
        glEnd();
    }

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
    {
        glColor3f(color.getX(), color.getY(), color.getZ());
        glBegin(GL_LINES);
        glVertex3f(from.getX(), from.getY(), from.getZ());
        glVertex3f(to.getX(), to.getY(), to.getZ());
        glEnd();
    }

    virtual void drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
    {
        glColor3f(color.getX(), color.getY(), color.getZ());
        glPushMatrix();
        glTranslatef(p.getX(), p.getY(), p.getZ());
        glutWireSphere(radius, 16, 16);
        glPopMatrix();
    }

    virtual void drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha)
    {
        glColor4f(color.getX(), color.getY(), color.getZ(), alpha);
        glBegin(GL_TRIANGLES);
        glVertex3f(a.getX(), a.getY(), a.getZ());
        glVertex3f(b.getX(), b.getY(), b.getZ());
        glVertex3f(c.getX(), c.getY(), c.getZ());
        glEnd();
    }

    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
    {
        glColor3f(color.getX(), color.getY(), color.getZ());
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        glVertex3f(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
        glEnd();
    }

    virtual void reportErrorWarning(const char* warningString) {}

    virtual void draw3dText(const btVector3& location, const char* textString) {}

    virtual void setDebugMode(int debugMode) { m_debugMode = debugMode; }

    virtual int getDebugMode() const { return m_debugMode; }
};
