#pragma once

#include "cinder/Cinder.h"
#include "cinder/Filesystem.h"

#include <dxsdkver.h>
#if ( _DXSDK_PRODUCT_MAJOR < 9 || _DXSDK_BUILD_MAJOR < 1949 )
#error The installed DXSDK is out of date.
#endif

#include <d3d11.h>
#include <d3dcompiler.h>

#include "cinder/Exception.h"
#include "cinder/Quaternion.h"
#include "cinder/Matrix.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/Rect.h"
#include "cinder/Font.h"
#include "cinder/PolyLine.h"
#include "cinder/AxisAlignedBox.h"
#include "cinder/Camera.h"

#include "d3dx11effect.h"

#include "V.h"

// forward declarations
namespace cinder {
	class Camera; class TriMesh2d; class TriMesh; class Sphere;
	namespace dx11 {
		 class VboMesh; class Texture;
	}
} // namespace cinder

namespace cinder { namespace dx11 {

class CameraPerspDX : public CameraPersp{
	void calcProjection();
};

void setDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_z_ const char *name);

ID3D11Device* getDevice();

ID3D11DeviceContext* getImmediateContext();

//! Clears the DX9 color buffer using \a color and optionally clears the depth buffer when \a clearDepthBuffer
void clear( const ColorA &color = ColorA::black(), bool clearDepthBuffer = true, float clearZValue = 1.0f);

HRESULT compileShader(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );

HRESULT compileShader(const Buffer& data, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );

HRESULT createShader(DataSourceRef datasrc, const char* entryName, const char* profileName, 
	ID3D11VertexShader** pVertexShader, ID3DBlob** pBlobOut = NULL);

HRESULT createShader(DataSourceRef datasrc, const char* entryName, const char* profileName, ID3D11PixelShader** pPixelShader);

void blendFunction(D3D11_BLEND  src, D3D11_BLEND dst);

//! Enables or disables wait for vertical sync
void enableVerticalSync( bool enable = true );
//! Disables wait for vertical sync
inline void disableVerticalSync() { enableVerticalSync( false ); }
//! Returns whether vertical sync is enabled for the current context
bool isVerticalSyncEnabled();

//! Sets the \c MODELVIEW and \c PROJECTION matrices to reflect the values of \a cam. Leaves the \c MatrixMode as \c MODELVIEW.
void setMatrices( const Camera &cam );
//! Sets the \c MODELVIEW matrix to reflect the values of \a cam. Leaves the \c MatrixMode as \c MODELVIEW.
void setModelView( const Camera &cam );
//! Sets the \c PROJECTION matrix to reflect the values of \a cam. Leaves the \c MatrixMode as \c PROJECTION.
void setProjection( const Camera &cam );
//! Pushes the \c MODELVIEW matrix onto its stack, preserving the current values. Leaves the \c MatrixMode as \c MODELVIEW.
void pushModelView();
//! Pops the \c MODELVIEW matrix off of its stack, restoring the values saved with the previous push. Leaves the \c MatrixMode as \c MODELVIEW.
void popModelView();
//! Pushes the \c MODELVIEW matrix onto its stack, preserving the current values, and then sets the matrix to reflect \a cam. Leaves the \c MatrixMode as \c MODELVIEW.
void pushModelView( const Camera &cam );
//! Pushes the \c PROJECTION matrix onto its stack, preserving the current values, and then sets the matrix to reflect \a cam. Leaves the \c MatrixMode as \c PROJECTION.
void pushProjection( const Camera &cam );
//! Pushes the \c MODELVIEW and \c PROJECTION matrices onto their stacks, preserving the current values. Leaves the \c MatrixMode as \c MODELVIEW.
void pushMatrices();
//! Pops the \c MODELVIEW and \c PROJECTION matrices off their stacks, restoring the values saved with the previous push. Leaves the \c MatrixMode as \c MODELVIEW.
void popMatrices();
//! Multiplies the current \c MODELVIEW matrix with \a mtx. Leaves the \c MatrixMode as \c MODELVIEW.
void multModelView( const Matrix44f &mtx );
//! Multiplies the current \c PROJECTION matrix with \a mtx. Leaves the \c MatrixMode as \c MODELVIEW.
void multProjection( const Matrix44f &mtx );
//! Returns the value of the current \c MODELVIEW matrix as a Matrix44f.
Matrix44f getModelView();
//! Returns the value of the current \c PROJECTION matrix as a Matrix44f.
Matrix44f getProjection();

//! Sets the viepwort and \c MODELVIEW and \c PROJECTION matrices to be a perspective projection with the upper-left corner at \c [0,0] and the lower-right at \c [screenWidth,screenHeight], but flipped vertically if not \a originUpperLeft.
void setMatricesWindowPersp( int screenWidth, int screenHeight, float fovDegrees = 60.0f, float nearPlane = 1.0f, float farPlane = 1000.0f, bool originUpperLeft = true );
//! Sets the viewport and \c MODELVIEW and \c PROJECTION matrices to be a perspective projection with the upper-left corner at \c [0,0] and the lower-right at \c [screenWidth,screenHeight], but flipped vertically if not \a originUpperLeft.
inline void setMatricesWindowPersp( const Vec2i &screenSize, float fovDegrees = 60.0f, float nearPlane = 1.0f, float farPlane = 1000.0f, bool originUpperLeft = true )
	{ setMatricesWindowPersp( screenSize.x, screenSize.y, fovDegrees, nearPlane, farPlane ); }
//! Sets the viewport and \c MODELVIEW and \c PROJECTION matrices to orthographic with the upper-left corner at \c [0,0] and the lower-right at \c [screenWidth,screenHeight] if \a originUpperLeft is \c true. Otherwise the origin is in the lower right.
void setMatricesWindow( int screenWidth, int screenHeight, bool originUpperLeft = true );
//! Sets the viewport and the \c MODELVIEW and \c PROJECTION matrices to orthographic with the upper-left corner at \c [0,0] and the lower-right at \c [size.x,size.y] if \a originUpperLeft is \c true. Otherwise the origin is in the lower right.
inline void setMatricesWindow( const Vec2i &screenSize, bool originUpperLeft = true ) { setMatricesWindow( screenSize.x, screenSize.y, originUpperLeft ); }

//! Returns the current OpenGL Viewport as an Area
Area getViewport();
//! Sets the current OpenGL Viewport to \a area
void setViewport( const Area &area );

//! Produces a translation by \a pos in the current matrix.
void translate( const Vec2f &pos );
//! Produces a translation by \a x and \a y in the current matrix.
inline void translate( float x, float y ) { translate( Vec2f( x, y ) ); }
//! Produces a translation by \a pos in the current matrix.
void translate( const Vec3f &pos );
//! Produces a translation by \a x, \a y and \a z in the current matrix.
inline void translate( float x, float y, float z ) { translate( Vec3f( x, y, z ) ); }

//! Produces a scale by \a scale in the current matrix.
void scale( const Vec3f &scl );
//! Produces a scale by \a scl in the current matrix.
inline void scale( const Vec2f &scl ) { scale( Vec3f( scl.x, scl.y, 0 ) ); }
//! Produces a scale by \a x and \a y in the current matrix.
inline void scale( float x, float y ) { scale( Vec3f( x, y, 0 ) ); }
//! Produces a scale by \a x, \a y and \a z in the current matrix.
inline void scale( float x, float y, float z ) { scale( Vec3f( x, y, z ) ); }

//! Produces a rotation around the X-axis by \a xyz.x degrees, the Y-axis by \a xyz.y degrees and the Z-axis by \a xyz.z degrees in the current matrix. Processed in X-Y-Z order.
void rotate( const Vec3f &xyz );
//! Produces a rotation by the quaternion \a quat in the current matrix.
void rotate( const Quatf &quat );
//! Produces a 2D rotation, the equivalent of a rotation around the Z axis by \a degrees.
inline void rotate( float degrees ) { rotate( Vec3f( 0, 0, degrees ) ); }

////! Enables the OpenGL State \a state. Equivalent to calling to glEnable( state );
//inline void enable( GLenum state ) { glEnable( state ); }
////! Disables the OpenGL State \a state. Equivalent to calling to glDisable( state );
//inline void disable( GLenum state ) { glDisable( state ); }

//! Enables alpha blending. Selects a \c BlendFunc that is appropriate for premultiplied-alpha when \a premultiplied
void enableAlphaBlending( bool premultiplied = false );
//! Disables alpha blending.
void disableAlphaBlending();
//! Enables alpha blending and selects a \c BlendFunc for additive blending
void enableAdditiveBlending();

//! Enables wireframe drawing by setting the \c PolygonMode to \c GL_LINE.
void enableWireframe();
//! Disables wireframe drawing.
void disableWireframe();

//! Disables reading from the depth buffer, disabling z-testing.
void disableDepthRead();
//! Disables writing to the depth buffer.
void disableDepthWrite();
//! Enables reading from the depth buffer when \a enable, enabling z-testing.
void enableDepthRead( bool enable = true );
//! Enables writing to the depth buffer when \a enable.
void enableDepthWrite( bool enable = true );

//! Draws a line from \a start to \a end
void drawLine( const Vec2f &start, const Vec2f &end );
//! Draws a line from \a start to \a end
void drawLine( const Vec3f &start, const Vec3f &end );
//! Renders a solid cube centered at \a center of size \a size. Normals and created texture coordinates are generated for \c GL_TEXTURE_2D, with each face in the range [0,0] - [1.0,1.0]
void drawCube( const Vec3f &center, const Vec3f &size );
//! Renders a solid cube centered at \a center of size \a size. Each face is assigned a unique color, and no normals or texture coordinates are generated.
void drawColorCube( const Vec3f &center, const Vec3f &size );
//! Renders a stroked cube centered at \a center of size \a size.
void drawStrokedCube( const Vec3f &center, const Vec3f &size );
//! Renders a stroked cube \a aab
inline void drawStrokedCube( const AxisAlignedBox3f &aab ) { drawStrokedCube( aab.getCenter(), aab.getSize() ); }
//! Renders a solid sphere centered at \a center of radius \a radius. \a segments defines how many segments the sphere is subdivided into. Normals and texture coordinates in the range [0,1] are generated.
void drawSphere( const Vec3f &center, float radius, int segments = 12 );
//! Renders a solid sphere. \a segments defines how many segments the sphere is subdivided into. Normals and texture coordinates in the range [0,1] are generated.
void draw( const class Sphere &sphere, int segments = 12 );
//! Renders a solid circle using triangle fans. The default value of zero for \a numSegments automatically determines a number of segments based on the circle's circumference.
void drawSolidCircle( const Vec2f &center, float radius, int numSegments = 0 );
//! Renders a stroked circle using a line loop. The default value of zero for \a numSegments automatically determines a number of segments based on the circle's circumference.
void drawStrokedCircle( const Vec2f &center, float radius, int numSegments = 0 );
//! Renders a solid ellipse using triangle fans. The default value of zero for \a numSegments automatically determines a number of segments based on the ellipse's circumference.
void drawSolidEllipse( const Vec2f &center, float radiusX, float radiusY, int numSegments = 0 );
//! Renders a stroked circle using a line loop. The default value of zero for \a numSegments automatically determines a number of segments based on the circle's circumference.
void drawStrokedEllipse( const Vec2f &center, float radiusX, float radiusY, int numSegments = 0 );
//! Renders a solid rectangle. Texture coordinates in the range [0,1] are generated unless \a textureRectangle.
void drawSolidRect( const Rectf &rect, bool textureRectangle = false );
//! Renders a stroked rectangle.
void drawStrokedRect( const Rectf &rect );
void drawSolidRoundedRect( const Rectf &r, float cornerRadius, int numSegmentsPerCorner = 0 );
void drawStrokedRoundedRect( const Rectf &r, float cornerRadius, int numSegmentsPerCorner = 0 );
//! Renders a coordinate frame representation centered at the origin. Arrowheads are drawn at the end of each axis with radius \a headRadius and length \a headLength.
void drawCoordinateFrame( float axisLength = 1.0f, float headLength = 0.2f, float headRadius = 0.05f );
//! Draws a vector starting at \a start and ending at \a end. An arrowhead is drawn at the end of radius \a headRadius and length \a headLength.
void drawVector( const Vec3f &start, const Vec3f &end, float headLength = 0.2f, float headRadius = 0.05f );
//! Draws a wireframe representation of the frustum defined by \a cam.
void drawFrustum( const Camera &cam );
//! Draws a torus at the origin, with an outter radius \a outterRadius and an inner radius \a innerRadius, subdivided into \a longitudeSegments and \a latitudeSegments. Normals and texture coordinates in the range [0,1] are generated.
void drawTorus( float outterRadius, float innerRadius, int longitudeSegments = 12, int latitudeSegments = 12 );
//! Draws a open-ended cylinder, with base radius \a baseRadius and top radius \a topRadius, with height \a height, subdivided into \a slices and \a stacks. Normals and texture coordinates in the range [0,1] are generated.
void drawCylinder( float baseRadius, float topRadius, float height, int slices = 12, int stacks = 1 );
//! Draws a 2d PolyLine \a polyLine
void draw( const class PolyLine<Vec2f> &polyLine );
//! Draws a 3d PolyLine \a polyLine
void draw( const class PolyLine<Vec3f> &polyLine );
//! Draws a Path2d \a path2d using approximation scale \a approximationScale. 1.0 corresponds to screenspace, 2.0 is double screen resolution, etc
void draw( const class Path2d &path2d, float approximationScale = 1.0f );
//! Draws a Shape2d \a shape2d using approximation scale \a approximationScale. 1.0 corresponds to screenspace, 2.0 is double screen resolution, etc
void draw( const class Shape2d &shape2d, float approximationScale = 1.0f );

//! Draws a solid (filled) Path2d \a path2d using approximation scale \a approximationScale. 1.0 corresponds to screenspace, 2.0 is double screen resolution, etc. Performance warning: This routine tesselates the polygon into triangles. Consider using Triangulator directly.
void drawSolid( const class Path2d &path2d, float approximationScale = 1.0f );
//! Draws a solid (filled) Shape2d \a shape2d using approximation scale \a approximationScale. 1.0 corresponds to screenspace, 2.0 is double screen resolution, etc. Performance warning: This routine tesselates the polygon into triangles. Consider using Triangulator directly.
void drawSolid( const class Shape2d &shape2d, float approximationScale = 1.0f );
//! Draws a solid (filled) PolyLine2f \a polyLine. Performance warning: This routine tesselates the polygon into triangles. Consider using Triangulator directly.
void drawSolid( const PolyLine2f &polyLine );

//! Draws a cinder::TriMesh \a mesh at the origin.
void draw( const TriMesh2d &mesh );
//! Draws a range of triangles starting with triangle # \a startTriangle and a count of \a triangleCount from cinder::TriMesh \a mesh at the origin.
void drawRange( const TriMesh2d &mesh, size_t startTriangle, size_t triangleCount );
//! Draws a cinder::TriMesh \a mesh at the origin.
void draw( const TriMesh &mesh );
//! Draws a range of triangles starting with triangle # \a startTriangle and a count of \a triangleCount from cinder::TriMesh \a mesh at the origin.
void drawRange( const TriMesh &mesh, size_t startTriangle, size_t triangleCount );
//! Draws a cinder::gl::VboMesh \a mesh at the origin.

void draw( const VboMesh &vbo );
//! Draws a range of vertices and elements of cinder::gl::VboMesh \a mesh at the origin. Default parameters for \a vertexStart and \a vertexEnd imply the VboMesh's full range of vertices.
void drawRange( const VboMesh &vbo, size_t startIndex, size_t indexCount, int vertexStart = -1, int vertexEnd = -1 );

void drawBillboard( const Vec3f &pos, const Vec2f &scale, float rotationDegrees, const Vec3f &bbRight, const Vec3f &bbUp );
//! Draws \a texture on the XY-plane
void draw( const Texture &texture );
//! Draws \a texture on the XY-plane at \a pos
void draw( const Texture &texture, const Vec2f &pos );
//! Draws \a texture on the XY-plane in the rectangle defined by \a rect
void draw( const Texture &texture, const Rectf &rect );
//! Draws the pixels inside \a srcArea of \a texture on the XY-plane in the rectangle defined by \a destRect
void draw( const Texture &texture, const Area &srcArea, const Rectf &destRect );

//! Draws a string \a str with its lower left corner located at \a pos. Optional \a font and \a color affect the style.
void drawString( const std::string &str, const Vec2f &pos, const ColorA &color = ColorA( 1, 1, 1, 1 ), Font font = Font() );
//! Draws a string \a str with the horizontal center of its baseline located at \a pos. Optional \a font and \a color affect the style
void drawStringCentered( const std::string &str, const Vec2f &pos, const ColorA &color = ColorA( 1, 1, 1, 1 ), Font font = Font() );
//! Draws a right-justified string \a str with the center of its  located at \a pos. Optional \a font and \a color affect the style
void drawStringRight( const std::string &str, const Vec2f &pos, const ColorA &color = ColorA( 1, 1, 1, 1 ), Font font = Font() );

//
////! Convenience class designed to push and pop the currently bound texture for a given texture unit
//struct SaveTextureBindState {
//	SaveTextureBindState( GLint target );
//	~SaveTextureBindState();
//  private:
//	GLint	mTarget;
//	GLint	mOldID;
//};
//
////! Convenience class designed to push and pop a boolean OpenGL state
//struct BoolState {
//	BoolState( GLint target );
//	~BoolState();
//  private:
//	GLint		mTarget;
//	GLboolean	mOldValue;
//};
//
////! Convenience class designed to push and pop a boolean OpenGL state
//struct ClientBoolState {
//	ClientBoolState( GLint target );
//	~ClientBoolState();
//  private:
//	GLint		mTarget;
//	GLboolean	mOldValue;
//};
//
////! Convenience class designed to push and pop the current color
//struct SaveColorState {
//	SaveColorState();
//	~SaveColorState();
//  private:
//	GLfloat		mOldValues[4];
//};
//
////! Convenience class which pushes and pops the currently bound framebuffer
//struct SaveFramebufferBinding {
//	SaveFramebufferBinding();
//	~SaveFramebufferBinding();
//  private:
//	GLint		mOldValue;
//};


class Exception : public cinder::Exception {
};

class ExceptionUnknownTarget : public Exception {
};

} } // namespace cinder::dx11