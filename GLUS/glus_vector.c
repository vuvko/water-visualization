/*
 * GLUS - OpenGL 3 and 4 Utilities. Copyright (C) 2010 - 2012 Norbert Nopper
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

#include "GL/glus.h"

GLUSfloat GLUSAPIENTRY glusAmountf(const GLUSfloat vector[3])
{
	return sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}
		
GLUSvoid GLUSAPIENTRY glusNormalizef(GLUSfloat vector[3])
{
	GLUSuint i;

	GLUSfloat a = glusAmountf(vector);

    if (a == 0.0f)
	{
    	return;
	}

	for (i = 0; i < 3; i++)
	{
    	vector[i] /= a;
	}
}

GLUSfloat GLUSAPIENTRY glusDotf(const GLUSfloat vector0[3], const GLUSfloat vector1[3])
{
	return vector0[0]*vector1[0] + vector0[1]*vector1[1] + vector0[2]*vector1[2];
}

GLUSvoid GLUSAPIENTRY glusCrossf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3])
{
	GLUSuint i;

	GLUSfloat	temp[3];
	
	temp[0] = vector0[1]*vector1[2] - vector0[2]*vector1[1];
	temp[1] = vector0[2]*vector1[0] - vector0[0]*vector1[2];
	temp[2] = vector0[0]*vector1[1] - vector0[1]*vector1[0];
	
	for (i = 0; i < 3; i++)
	{
    	result[i] = temp[i];
	}
}


GLUSvoid GLUSAPIENTRY glusVector3Copyf(GLUSfloat result[3], const GLUSfloat vector[3])
{
    result[0] = vector[0];
    result[1] = vector[1];
    result[2] = vector[2];
}

GLUSvoid GLUSAPIENTRY glusVector2Copyf(GLUSfloat result[2], const GLUSfloat vector[2])
{
    result[0] = vector[0];
    result[1] = vector[1];
}

GLUSvoid GLUSAPIENTRY glusVector3AddVector3f(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3])
{
    result[0] = vector0[0] + vector1[0];
    result[1] = vector0[1] + vector1[1];
    result[2] = vector0[2] + vector1[2];
}

GLUSvoid GLUSAPIENTRY glusVector2AddVector2f(GLUSfloat result[2], const GLUSfloat vector0[2], const GLUSfloat vector1[2])
{
    result[0] = vector0[0] + vector1[0];
    result[1] = vector0[1] + vector1[1];
}

GLUSvoid GLUSAPIENTRY glusVector3SubtractVector3f(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3])
{
    result[0] = vector0[0] - vector1[0];
    result[1] = vector0[1] - vector1[1];
    result[2] = vector0[2] - vector1[2];
}

GLUSvoid GLUSAPIENTRY glusVector2SubtractVector2f(GLUSfloat result[2], const GLUSfloat vector0[2], const GLUSfloat vector1[2])
{
    result[0] = vector0[0] - vector1[0];
    result[1] = vector0[1] - vector1[1];
}

GLUSvoid GLUSAPIENTRY glusVector3MultiplyScalarf(GLUSfloat result[3], const GLUSfloat vector[3], const GLUSfloat scalar)
{
    result[0] = vector[0] * scalar;
    result[1] = vector[1] * scalar;
    result[2] = vector[2] * scalar;
}

GLUSvoid GLUSAPIENTRY glusVector2MultiplyScalarf(GLUSfloat result[2], const GLUSfloat vector[2], const GLUSfloat scalar)
{
    result[0] = vector[0] * scalar;
    result[1] = vector[1] * scalar;
}

GLUSfloat GLUSAPIENTRY glusVector3Lengthf(const GLUSfloat vector[3])
{
    return sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

GLUSfloat GLUSAPIENTRY glusVector2Lengthf(const GLUSfloat vector[2])
{
    return sqrtf(vector[0] * vector[0] + vector[1] * vector[1]);
}

GLUSboolean GLUSAPIENTRY glusVector3Normalizef(GLUSfloat vector[3])
{
    GLUSint i;

    GLUSfloat length = glusVector3Lengthf(vector);

    if (length == 0.0f)
    {
        return GLUS_FALSE;
    }

    for (i = 0; i < 3; i++)
    {
        vector[i] /= length;
    }

    return GLUS_TRUE;
}

GLUSboolean GLUSAPIENTRY glusVector2Normalizef(GLUSfloat vector[2])
{
    GLUSint i;

    GLUSfloat length = glusVector2Lengthf(vector);

    if (length == 0.0f)
    {
        return GLUS_FALSE;
    }

    for (i = 0; i < 2; i++)
    {
        vector[i] /= length;
    }

    return GLUS_TRUE;
}

GLUSfloat GLUSAPIENTRY glusVector3Dotf(const GLUSfloat vector0[3], const GLUSfloat vector1[3])
{
    return vector0[0] * vector1[0] + vector0[1] * vector1[1] + vector0[2] * vector1[2];
}

GLUSfloat GLUSAPIENTRY glusVector2Dotf(const GLUSfloat vector0[2], const GLUSfloat vector1[2])
{
    return vector0[0] * vector1[0] + vector0[1] * vector1[1];
}

GLUSvoid GLUSAPIENTRY glusVector3Crossf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3])
{
    GLUSint i;

    GLUSfloat temp[3];

    temp[0] = vector0[1] * vector1[2] - vector0[2] * vector1[1];
    temp[1] = vector0[2] * vector1[0] - vector0[0] * vector1[2];
    temp[2] = vector0[0] * vector1[1] - vector0[1] * vector1[0];

    for (i = 0; i < 3; i++)
    {
        result[i] = temp[i];
    }
}

GLUSvoid GLUSAPIENTRY glusVector3GetQuaternionf(GLUSfloat result[4], const GLUSfloat vector[3])
{
    result[0] = vector[0];
    result[1] = vector[1];
    result[2] = vector[2];
    result[3] = 0.0f;
}

GLUSvoid GLUSAPIENTRY glusVector3GetPoint4f(GLUSfloat result[4], const GLUSfloat vector[3])
{
    result[0] = vector[0];
    result[1] = vector[1];
    result[2] = vector[2];
    result[3] = 1.0f;
}

GLUSvoid GLUSAPIENTRY glusVector2GetPoint3f(GLUSfloat result[3], const GLUSfloat vector[2])
{
    result[0] = vector[0];
    result[1] = vector[1];
    result[2] = 1.0f;
}
