/**
 * GLUS - OpenGL 3 Utilities. Copyright (C) 2010 Norbert Nopper
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __glus_h_
#define __glus_h_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <gl/glew.h>
#include <GL/GL.h>


#ifndef GLUSAPIENTRY
#define GLUSAPIENTRY
#endif
#ifndef GLUSAPIENTRYP
#define GLUSAPIENTRYP GLUSAPIENTRY *
#endif
#ifndef GLUSAPI
#define GLUSAPI extern
#endif

typedef unsigned int GLUSenum;
typedef unsigned char GLUSboolean;
typedef unsigned int GLUSbitfield;
typedef signed char GLUSbyte;
typedef short GLUSshort;
typedef int GLUSint;
typedef int GLUSsizei;
typedef unsigned char GLUSubyte;
typedef unsigned short GLUSushort;
typedef unsigned int GLUSuint;
typedef float GLUSfloat;
typedef float GLUSclampf;
typedef double GLUSdouble;
typedef double GLUSclampd;
typedef char GLUSchar;

#ifdef __cplusplus
#define GLUSvoid void
#else
typedef void GLUSvoid;
#endif

#define GLUS_ALPHA 0x1906
#define GLUS_RGB 0x1907
#define GLUS_RGBA 0x1908

#define PIf		3.1415926535897932384626433832795f
				
typedef struct _GLUStextfile
{
	GLUSchar* text;

	GLUSint	length;

} GLUStextfile;

typedef struct _GLUStgaimage
{
	GLUSushort	width;

	GLUSushort	height;

	GLUSubyte*	data;

	GLUSenum 	format;

} GLUStgaimage;

typedef struct _GLUSshaderprogram
{
	GLUSuint	program;

	GLUSuint	vertex;

	GLUSuint	control;

	GLUSint		evaluation;

	GLUSuint	geometry;

	GLUSuint	fragment;

} GLUSshaderprogram;
/*
typedef struct _GLUSshape
{
	GLUSfloat*	vertices;
	
	GLUSfloat*	normals;
	
	GLUSfloat*	tangents;
	
	GLUSfloat*  texCoords;
	
	GLUSuint*	indices;

	GLUSuint	numberVertices;

	GLUSuint	numberIndices;

} GLUSshape;
*/

/**
 * Structure for holding geometry data.
 */
typedef struct _GLUSshape
{
	/**
	 * Vertices in homogeneous coordinates.
	 */
    GLUSfloat* vertices;

    /**
     * Normals.
     */
    GLUSfloat* normals;

    /**
     * Tangents.
     */
    GLUSfloat* tangents;

    /**
     * Bitangents.
     */
    GLUSfloat* bitangents;

    /**
     * Texture coordinates.
     */
    GLUSfloat* texCoords;

    /**
     * All above values in one array. Not created by the model loader.
     */
    GLUSfloat* allAttributes;

    /**
     * Indices.
     */
    GLUSuint* indices;

    /**
     * Number of vertices.
     */
    GLUSuint numberVertices;

    /**
     * Number of indices.
     */
    GLUSuint numberIndices;

    /**
     * Triangle render mode - could be either:
     *
     * GL_TRIANGLES
     * GL_TRIANGLE_STRIP
     */
    GLUSenum mode;

} GLUSshape;

#define GLUS_TRUE	1
#define GLUS_FALSE	0

#define GLUS_BACKWARD_COMPATIBLE_BIT	0x0000
#define GLUS_FORWARD_COMPATIBLE_BIT		0x0002

GLUSAPI GLUSvoid GLUSAPIENTRY glusInitFunc( GLUSboolean (*glusNewInit)(GLUSvoid) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusReshapeFunc( GLUSvoid (*glusNewReshape)(GLUSuint width, GLUSuint height) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusUpdateFunc( GLUSboolean (*glusNewUpdate)(GLUSfloat time) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusTerminateFunc( GLUSvoid (*glusNewTerminate)(GLUSvoid) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusPrepareContext(GLUSuint major, GLUSuint minor, GLUSint flags);

GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateWindow(const GLUSchar* title, GLUSuint width, GLUSuint height, GLUSboolean fullscreen);

GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyWindow(GLUSvoid);

GLUSAPI GLUSboolean GLUSAPIENTRY glusRun(GLUSvoid);

//

GLUSAPI GLUSvoid GLUSAPIENTRY glusKeyFunc( GLUSvoid (*glusNewKey)(GLUSboolean pressed, GLUSuint key) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusMouseFunc( GLUSvoid (*glusNewMouse)(GLUSboolean pressed, GLUSuint button, GLUSuint xPos, GLUSuint yPos) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusMouseWheelFunc( GLUSvoid (*glusNewMouseWheel)(GLUSuint buttons, GLUSint ticks, GLUSuint xPos, GLUSuint yPos) );

GLUSAPI GLUSvoid GLUSAPIENTRY glusMouseMoveFunc( GLUSvoid (*glusNewMouseMove)(GLUSuint buttons, GLUSint xPos, GLUSint yPos) );

//

GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadTextFile(const GLUSchar* filename, GLUStextfile* textfile);

GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyTextFile(GLUStextfile* textfile);

GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadTgaImage(const GLUSchar* filename, GLUStgaimage* tgaimage);
GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadBmpImage(const GLUSchar* filename, GLUStgaimage* tgaimage);

GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyTgaImage(GLUStgaimage* tgaimage);

//

GLUSAPI GLUSfloat GLUSAPIENTRY glusAmountf(const GLUSfloat vector[3]);
		
GLUSAPI GLUSvoid GLUSAPIENTRY glusNormalizef(GLUSfloat vector[3]);

GLUSAPI GLUSfloat GLUSAPIENTRY glusDotf(const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCrossf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

//
// Vector functions.
//

/**
 * Copies a 3D Vector.
 *
 * @param result The destination vector.
 * @param vector The source vector.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector3Copyf(GLUSfloat result[3], const GLUSfloat vector[3]);

/**
 * Copies a 2D Vector.
 *
 * @param result The destination vector.
 * @param vector The source vector.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector2Copyf(GLUSfloat result[2], const GLUSfloat vector[2]);

/**
 * Adds a 3D Vector to another.
 *
 * @param result The final vector.
 * @param vector0 The first vector.
 * @param vector1 The second vector.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector3AddVector3f(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

/**
 * Adds a 2D Vector to another.
 *
 * @param result The final vector.
 * @param vector0 The first vector.
 * @param vector1 The second vector.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector2AddVector2f(GLUSfloat result[2], const GLUSfloat vector0[2], const GLUSfloat vector1[2]);

/**
 * Subtracts a 3D Vector from another.
 *
 * @param result The final vector.
 * @param vector0 The vector subtracted by vector1.
 * @param vector1 The vector subtracted from vector0.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector3SubtractVector3f(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

/**
 * Subtracts a 2D Vector from another.
 *
 * @param result The final vector.
 * @param vector0 The vector subtracted by vector1.
 * @param vector1 The vector subtracted from vector0.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector2SubtractVector2f(GLUSfloat result[2], const GLUSfloat vector0[2], const GLUSfloat vector1[2]);

/**
 * Multiplies a 3D Vector by a scalar.
 *
 * @param result The final vector.
 * @param vector The used vector for multiplication.
 * @param scalar The scalar.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector3MultiplyScalarf(GLUSfloat result[3], const GLUSfloat vector[3], const GLUSfloat scalar);

/**
 * Multiplies a 2D Vector by a scalar.
 *
 * @param result The final vector.
 * @param vector The used vector for multiplication.
 * @param scalar The scalar.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector2MultiplyScalarf(GLUSfloat result[2], const GLUSfloat vector[2], const GLUSfloat scalar);

/**
 * Calculates the length of a 3D Vector.
 *
 * @param vector The used vector.
 *
 * @return The length of the vector.
 */
GLUSAPI GLUSfloat GLUSAPIENTRY glusVector3Lengthf(const GLUSfloat vector[3]);

/**
 * Calculates the length of a 2D Vector.
 *
 * @param vector The used vector.
 *
 * @return The length of the vector.
 */
GLUSAPI GLUSfloat GLUSAPIENTRY glusVector2Lengthf(const GLUSfloat vector[2]);

/**
 * Normalizes the given 3D Vector.
 *
 * @param vector The vector to normalize.
 *
 * @return GLUS_TRUE, if normalization succeeded.
 */
GLUSAPI GLUSboolean GLUSAPIENTRY glusVector3Normalizef(GLUSfloat vector[3]);

/**
 * Normalizes the given 2D Vector.
 *
 * @param vector The vector to normalize.
 *
 * @return GLUS_TRUE, if normalization succeeded.
 */
GLUSAPI GLUSboolean GLUSAPIENTRY glusVector2Normalizef(GLUSfloat vector[2]);

/**
 * Calculates the dot product of two 3D vectors.
 *
 * @param vector0 The first vector.
 * @param vector1 The second vector.
 *
 * @return The dot product.
 */
GLUSAPI GLUSfloat GLUSAPIENTRY glusVector3Dotf(const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

/**
 * Calculates the dot product of two 2D vectors.
 *
 * @param vector0 The first vector.
 * @param vector1 The second vector.
 *
 * @return The dot product.
 */
GLUSAPI GLUSfloat GLUSAPIENTRY glusVector2Dotf(const GLUSfloat vector0[2], const GLUSfloat vector1[2]);

/**
 * Calculates the cross product of two 3D vectors: vector0 x vector1.
 *
 * @param result The resulting vector from the cross product.
 * @param vector0 The first vector.
 * @param vector1 The second vector.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector3Crossf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

/**
 * Creates a quaternion out of a 3D vector.
 *
 * @param result The resulting quaternion.
 * @param vector The used vector.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector3GetQuaternionf(GLUSfloat result[4], const GLUSfloat vector[3]);

/**
 * Creates a 3D point, given as homogeneous coordinates, out of a 3D vector.
 *
 * @param result The resulting point.
 * @param vector The used vector.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector3GetPoint4f(GLUSfloat result[4], const GLUSfloat vector[3]);

/**
 * Creates a 2D point, given as homogeneous coordinates, out of a 2D vector.
 *
 * @param result The resulting point.
 * @param vector The used vector.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusVector2GetPoint3f(GLUSfloat result[3], const GLUSfloat vector[2]);


//

GLUSAPI GLUSvoid GLUSAPIENTRY glusOrthof(GLUSfloat result[16], GLUSfloat left, GLUSfloat right, GLUSfloat bottom, GLUSfloat top, GLUSfloat nearVal, GLUSfloat farVal);

GLUSAPI GLUSvoid GLUSAPIENTRY glusFrustumf(GLUSfloat result[16], GLUSfloat left, GLUSfloat right, GLUSfloat bottom, GLUSfloat top, GLUSfloat nearVal, GLUSfloat farVal);

GLUSAPI GLUSvoid GLUSAPIENTRY glusPerspectivef(GLUSfloat result[16], GLUSfloat fovy, GLUSfloat aspect, GLUSfloat zNear, GLUSfloat zFar);

GLUSAPI GLUSvoid GLUSAPIENTRY glusLookAtf(GLUSfloat result[16], GLUSfloat eyeX, GLUSfloat eyeY, GLUSfloat eyeZ, GLUSfloat centerX, GLUSfloat centerY, GLUSfloat centerZ, GLUSfloat upX, GLUSfloat upY, GLUSfloat upZ);

//

GLUSAPI GLUSvoid GLUSAPIENTRY glusLoadIdentityf(GLUSfloat matrix[16]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCopyMatrixf(GLUSfloat matrix[16], const GLUSfloat source[16]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusMultMatrixf(GLUSfloat matrix[16], const GLUSfloat matrix0[16], const GLUSfloat matrix1[16]);

GLUSAPI GLUSboolean GLUSAPIENTRY glusInverseMatrixf(GLUSfloat matrix[16], const GLUSfloat source[16]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusTransposef(GLUSfloat matrix[16]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusTranslatef(GLUSfloat matrix[16], GLUSfloat x, GLUSfloat y, GLUSfloat z);

GLUSAPI GLUSvoid GLUSAPIENTRY glusRotatef(GLUSfloat matrix[16], GLUSfloat angle, GLUSfloat x, GLUSfloat y, GLUSfloat z);

GLUSAPI GLUSvoid GLUSAPIENTRY glusRotateRzRyRxf(GLUSfloat matrix[16], GLUSfloat anglex, GLUSfloat angley, GLUSfloat anglez);

GLUSAPI GLUSvoid GLUSAPIENTRY glusScalef(GLUSfloat matrix[16], GLUSfloat x, GLUSfloat y, GLUSfloat z);

GLUSAPI GLUSvoid GLUSAPIENTRY glusMultVectorf(GLUSfloat result[3], const GLUSfloat matrix[16], const GLUSfloat vector[3]);

GLUSAPI GLUSvoid GLUSAPIENTRY glusMultPointf(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat vector[4]);

//

GLUSAPI GLUSboolean GLUSAPIENTRY glusBuildProgram(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource);

GLUSAPI GLUSboolean GLUSAPIENTRY glusCompileProgram(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource);

GLUSAPI GLUSboolean GLUSAPIENTRY glusLinkProgram(GLUSshaderprogram* shaderProgram);

GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyProgram(GLUSshaderprogram* shaderprogram);

//

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreatePlanef(GLUSshape* shape, GLUSfloat halfExtend);
GLUSAPI GLUSvoid GLUSAPIENTRY glusCreatePlaneSlicedf(GLUSshape* shape, GLUSfloat halfExtend, GLUSuint numberSlices);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateRectangularPlanef(GLUSshape* shape, GLUSfloat horizontalExtend, GLUSfloat verticalExtend);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateCubef(GLUSshape* shape, GLUSfloat halfExtend);
GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateCubeOpenf(GLUSshape* shape, GLUSfloat halfExtend);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateSpheref(GLUSshape* shape, GLUSfloat radius, GLUSuint numberSlices);

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateTorusf(GLUSshape* shape, GLUSfloat innerRadius, GLUSfloat outerRadius, GLUSuint numSides, GLUSuint numFaces);

GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyShapef(GLUSshape* shape);

GLUSAPI GLUSvoid GLUSAPIENTRY glusPrintErrorMsg(GLenum);

/**
 * Calculates and creates the tangent and bitangent vectors. Uses the previous created memory for the tangents and bitangents.
 *
 * @param shape The structure which will be filled with the calculated vectors.
 *
 * @return GLUS_TRUE, if creation succeeded.
 */
GLUSAPI GLUSboolean GLUSAPIENTRY glusCalculateTangentSpacef(GLUSshape* shape);

//
// Point functions.
//

/**
 * Copies a 3D point, given as homogeneous coordinates.
 *
 * @param result The destination point.
 * @param point The source point.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint4Copyf(GLUSfloat result[4], const GLUSfloat point[4]);

/**
 * Copies a 2D point, given as homogeneous coordinates.
 *
 * @param result The destination point.
 * @param point The source point.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint3Copyf(GLUSfloat result[3], const GLUSfloat point[3]);

/**
 * Subtracts a 3D point, given as homogeneous coordinates, from another and calculates a 3D vector.
 *
 * @param result The resulting vector.
 * @param point0 The point subtracted by point1.
 * @param point1 The point subtracted from point0.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint4SubtractPoint4f(GLUSfloat result[3], const GLUSfloat point0[4], const GLUSfloat point1[4]);

/**
 * Subtracts a 2D point, given as homogeneous coordinates, from another and calculates a 2D vector.
 *
 * @param result The resulting vector.
 * @param point0 The point subtracted by point1.
 * @param point1 The point subtracted from point0.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint3SubtractPoint3f(GLUSfloat result[2], const GLUSfloat point0[3], const GLUSfloat point1[3]);

/**
 * Adds a vector to a 3D point, given as homogeneous coordinates. Result is the new point.
 *
 * @param result The resulting point.
 * @param point The point.
 * @param vector The vector, which is added to the point.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint4AddVector3f(GLUSfloat result[4], const GLUSfloat point[4], const GLUSfloat vector[3]);

/**
 * Adds a vector to a 2D point, given as homogeneous coordinates. Result is the new point.
 *
 * @param result The resulting point.
 * @param point The point.
 * @param vector The vector, which is added to the point.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint3AddVector2f(GLUSfloat result[3], const GLUSfloat point[3], const GLUSfloat vector[2]);

/**
 * Subtracts a vector from a 3D point, given as homogeneous coordinates. Result is the new point.
 *
 * @param result The resulting point.
 * @param point The point.
 * @param vector The vector, which is subtracted from the point.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint4SubtractVector3f(GLUSfloat result[4], const GLUSfloat point[4], const GLUSfloat vector[3]);

/**
 * Subtracts a vector from a 2D point, given as homogeneous coordinates. Result is the new point.
 *
 * @param result The resulting point.
 * @param point The point.
 * @param vector The vector, which is subtracted from the point.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint3SubtractVector2f(GLUSfloat result[3], const GLUSfloat point[3], const GLUSfloat vector[2]);

/**
 * Converts a 3D point, given as homogeneous coordinates, to a quaternion.
 *
 * @param result The resulting quaternion.
 * @param point The point, which is converted.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint4GetQuaternionf(GLUSfloat result[4], const GLUSfloat point[4]);

/**
 * Converts a 3D point, given as homogeneous coordinates, to a vector.
 *
 * @param result The resulting vector.
 * @param point The point, which is converted.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint4GetVector3f(GLUSfloat result[3], const GLUSfloat point[4]);

/**
 * Converts a 2D point, given as homogeneous coordinates, to a vector.
 *
 * @param result The resulting vector.
 * @param point The point, which is converted.
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusPoint3GetVector2f(GLUSfloat result[2], const GLUSfloat point[3]);

/**
 * Calculates the distance of two 3D points, given as homogeneous coordinates.
 *
 * @param point0 The first point.
 * @param point1 The second point.
 *
 * @return The distance of the two points.
 */
GLUSAPI GLUSfloat GLUSAPIENTRY glusPoint4Distancef(const GLUSfloat point0[4], const GLUSfloat point1[4]);

/**
 * Calculates the distance of two 2D points, given as homogeneous coordinates.
 *
 * @param point0 The first point.
 * @param point1 The second point.
 *
 * @return The distance of the two points.
 */
GLUSAPI GLUSfloat GLUSAPIENTRY glusPoint3Distancef(const GLUSfloat point0[3], const GLUSfloat point1[3]);

//
// Model loading functions.
//

/**
 * Loads a wavefront object file.
 *
 * @param filename The name of the wavefront file including extension.
 * @param shape The data is stored into this structure.
 *
 * @return GLUS_TRUE, if loading succeeded.
 */
GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadObjFile(const GLUSchar* filename, GLUSshape* shape);


#ifdef __cplusplus
}
#endif


// проверка на ошибки OpenGL
//#define OGL_CHECK_FOR_ERRORS \
//{ \
//  GLenum openGLError; \
//  if ((openGLError = glGetError()) != GL_NO_ERROR) \
//  { \
//    glusPrintErrorMsg(openGLError); \
//    fprintf(stderr, "file %s, \nline %d\n", __FILE__, __LINE__); \
//  } \
//} \

// безопасный вызов функции OpenGL
//#define OGL_SAFE_CALL(expression) \
//{ \
//  expression; \
//  GLenum openGLError; \
//  if ((openGLError = glGetError()) != GL_NO_ERROR) \
//  fprintf(stderr, "OpenGL call failed \"" #expression "\" error %d\n", (int)openGLError); \
//}





#endif /*__glus_h_*/
