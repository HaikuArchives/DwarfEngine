
/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include <UTF8.h>
#include <String.h>
#include <Screen.h>
#include <View.h>
#include <GLView.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <Rect.h>

#include "SoftwareDevice.h"
#include "Plane.h"
#include "Vertex.h"
#include "Quaternion.h"
#include "Rect.h"
#include "Color.h"
#include "Window.h"
#include "Array.h"
#include "InputServer.h"
#include "InputServerBase.h"
#include "Matrix4x4.h"
#include "OpenGLWindow.h"
#include "VertexArray.h"

using namespace dwarf;

/********************************************************************/
// SoftwareDevice

// checks to see if x = 2^y
namespace
{
	bool israised2(int32 x)
	{
		if (x == 0)
			return false;

		int32 y;
		for (int32 i = 0; i < 32; i++)
		{
			y = x & (1 << i);
			if (y) return y == x;
		}
		return false;
	}
}

void SoftwareDevice::Viewport(const Rect &rect)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_viewport = rect;
}

void SoftwareDevice::Scissor(const Rect &rect)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(false, TEXT("UNIMPLEMENTED!"));
}

void SoftwareDevice::LoadIdentity()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	ModelMatrix() = IdentityMatrix();
	m_identity_model = true;
}

void SoftwareDevice::Translate(const Vertex<3> &point)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	ModelMatrix()[12] += point.x();
	ModelMatrix()[13] += point.y();
	ModelMatrix()[14] += point.z();
	m_identity_model = false;
}

void SoftwareDevice::Rotate(const Quaternion &rotation)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	Matrix4x4 m = rotation.ToMatrix();
	m_model_matrix[m_model_stack_ptr] *= m;
	m_identity_model = false;
}

void SoftwareDevice::MultMatrix(const Matrix4x4 &matrix)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_model_matrix[m_model_stack_ptr] *= matrix;
	m_identity_model = false;
}


void SoftwareDevice::Scale(const Vertex<3> &factor)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	ModelMatrix()[0] *= factor.x();
	ModelMatrix()[5] *= factor.y();
	ModelMatrix()[10] *= factor.z();
	m_identity_model = false;
}

void SoftwareDevice::SetTextureFilters(HTexture texture)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(texture.IsValid(), TEXT("You are using an uninitialized texture."));

	D_ASSERT_MESSAGE(false, TEXT("UNIMPLEMENTED!"));
/*
    // set up correction mode
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, correction[texture->CorrectionMode()]);

    // set up apply mode
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, apply[texture->ApplyMode()]);

    glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR, texture->BlendColor());

    // set up wrap mode
    switch ( texture->WrapMode() )
    {
    case Texture::CLAMP_S_CLAMP_T:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
        break;
    case Texture::CLAMP_S_WRAP_T:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    case Texture::WRAP_S_CLAMP_T:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
        break;
    case Texture::WRAP_S_WRAP_T:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    }

	// set up filter mode
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, filter[texture->FilterMode()]);

	// set up mipmap mode
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, mipmap[texture->MipmapMode()]);
*/
}

void SoftwareDevice::UseTexture(int32 texture_unit, HTexture texture) // sets the given texture, uploading it if it isn't on the card
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(texture.IsValid(), TEXT("Invalid texture given to SoftwareDevice::UseTexture."));
	D_ASSERT_MESSAGE(!m_in_begin, TEXT("You cannot change texture between Begin() and End()"));

	if (!texture->IsResident())
	{
		UploadTexture(texture);
	}

	m_current_state.SetTexture(texture);
//	logger(TEXT("UseTexture: %3d TexUnit: %3d"), texture->Index(), texture_unit);

	if (texture_unit < 1)
	{
		m_current_texture = reinterpret_cast<rgba_color*>(texture->GetBitmap()->Data());
		m_current_texture_width = texture->GetBitmap()->Width() - 1;
		m_current_texture_height = texture->GetBitmap()->Height() - 1;
	}
	else
	{
		D_ASSERT_MESSAGE(texture_unit < 1, TEXT("Multitexturing not yet supported."));
	}
}

void SoftwareDevice::UploadTexture(HTexture texture) // uploads the given texture to the card
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(texture.IsValid(), TEXT("Invalid handle given to UploadTexture!"));

	if (texture->IsResident())
	{
		RemoveTexture(texture);
	}

	D_ASSERT_MESSAGE(texture->GetBitmap(), TEXT("UploadTexture() called on texture with no bitmap!"));

	int32 tex_name = m_textures.AddItem(texture->GetBitmap());

//	SetTextureFilters(texture);

	texture->SetIndex(tex_name);
	
	logger(TEXT("Texture index %d assigned to (%dx%d, %d)"), 
		texture->Index(), texture->Width(), texture->Height(), texture->GetBitmap()->BytesPerPixel());
}

void SoftwareDevice::RemoveTexture(HTexture texture) // removes the given texture from the card
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(texture.IsValid(), TEXT("Invalid handle given to RemoveTexture!"));
	if (!texture->IsResident()) return;			// texture was non resident (does not reside on the card)
	
	int32 tex_name = texture->Index();
	texture->SetIndex(-1);
	m_textures.RemoveItemAt(tex_name);
}


void SoftwareDevice::SetDepthRange(float min, float max)
{
	D_ASSERT_MESSAGE(false, TEXT("UNIMPLEMENTED!"));
}




void SoftwareDevice::SetMaterialState(HMaterial material)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(material.IsValid(), TEXT("Invalid material given to SetMaterialState"));

	D_ASSERT_MESSAGE(false, TEXT("UNIMPLEMENTED!"));
}

void SoftwareDevice::CreateLight(int32 index, Vertex<3> const& pos, Color const& color)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(false, TEXT("UNIMPLEMENTED!"));
}

void SoftwareDevice::DisableLight(int32 index)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(false, TEXT("UNIMPLEMENTED!"));
}



enum LayerFlags
{
	LF_TEXCOORD_ARRAY	= 0x00000001,
	LF_NORMAL_ARRAY		= 0x00000002,
	LF_COLOR_ARRA		= 0x00000004,
	LF_INDEX_ARRAY		= 0x00000008,
	LF_EDGEFLAG_ARRAY	= 0x00000010
};

void SoftwareDevice::SetClipPlane(const Plane<3> &plane)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(false, TEXT("UNIMPLEMENTED!"));
}

void SoftwareDevice::SetLayerState(const Shader::Layer &layer, int32 flags)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	// *** select the texture *******

	if (layer.m_textures.ItemCount() > 0)
	{
		if(layer.m_is_animated)
		{
			UseTexture(0,layer.m_textures[(uint32)fmod(m_time*layer.m_framerate,layer.m_textures.ItemCount())]);
		}
		else UseTexture(0, layer.m_textures[0]); // no animation yet
		EnableState(TEXTURE_2D);	
		//		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		DisableState(TEXTURE_2D);
	}

	// *** set the blendmode *********
	if (layer.m_blend_dst == ZERO && layer.m_blend_src == ONE)
	{
		DisableState(BLEND);
	}
	else
	{
		EnableState(BLEND);
		SetBlendMode((RenderingDevice::Blendmode)layer.m_blend_src, (RenderingDevice::Blendmode)layer.m_blend_dst);
	}

	// *** set the fillmode **********
	SetPolygonMode((RenderingDevice::Fillmode)layer.m_fillmode);
//	static const int32 fill_table[] = {GL_POINT, GL_LINE, GL_FILL};
//	glPolygonMode(GL_FRONT_AND_BACK, fill_table[layer.m_fillmode]);

	// *** set cull mode ********
	if (layer.m_cull)
	{
		SetCullMode((RenderingDevice::Faces)layer.m_cull_faces);
//		static const int32 face_table[] = {GL_FRONT, GL_BACK, GL_FRONT_AND_BACK};
//		glCullFace(face_table[layer.m_cull_faces]);
		EnableState(CULL_FACE);
	}
	else
	{
		DisableState(CULL_FACE);
	}

	// *** set the depthfunction *******
	SetDepthFunc((RenderingDevice::TestFunction)layer.m_depth_func);
//	static const int32 func_table[] =
//		{GL_NEVER,GL_LESS,GL_EQUAL,GL_LEQUAL,GL_GREATER,GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS};
//	glDepthFunc(func_table[layer.m_depth_func]);


	// *** set the generated alpha *******

	Color color;

	color.r = layer.m_rgba_wave[0](m_time);
	color.g = layer.m_rgba_wave[1](m_time);
	color.b = layer.m_rgba_wave[2](m_time);
	color.a = layer.m_rgba_wave[3](m_time);

	/*switch(layer.m_rgb_wave)
	{
		float alpha;	
	case Shader::Layer::SIN:
		color = layer.m_color * (((sin((m_time + layer.m_rgb_phase) * D_PI * layer.m_rgb_frequency) + 1.0) * 0.5f * layer.m_rgb_amplitude) + layer.m_rgb_bias);
		break;
	case Shader::Layer::SQUARE:
		color = layer.m_color * (((fmod(m_time + layer.m_rgb_phase, 1.0f/layer.m_rgb_frequency) > (1.0f/layer.m_rgb_frequency * layer.m_rgb_pwm)) ? 0.0f : layer.m_rgb_amplitude) + layer.m_rgb_bias);
		break;
	case Shader::Layer::TRIANGLE:
		alpha = (fmod(m_time + layer.m_rgb_phase, 1.0f/layer.m_rgb_frequency) / (1.0f/layer.m_rgb_frequency)) * 2.0f;
		color = layer.m_color * ((alpha > 1.0f ? 2.0f - alpha : alpha) * layer.m_rgb_amplitude + layer.m_rgb_bias);
		break;
	case Shader::Layer::SAWTOOTH:
		color = layer.m_color * (fmod((m_time + layer.m_rgb_phase) * layer.m_rgb_frequency * layer.m_rgb_amplitude,layer.m_rgb_amplitude) + layer.m_rgb_bias);
		break;
	case Shader::Layer::INVERSE_SAWTOOTH:
		color = layer.m_color * (layer.m_rgb_amplitude - (fmod((m_time + layer.m_rgb_phase) * layer.m_rgb_frequency * layer.m_rgb_amplitude,layer.m_rgb_amplitude) + layer.m_rgb_bias));
		break;
	case Shader::Layer::NONE:
	default:
		color = layer.m_color;
		break;	
	}

	switch(layer.m_alpha_wave)
	{
		float alpha;	
	case Shader::Layer::SIN:
		color.a = ((sin((m_time + layer.m_alpha_phase) * D_PI * layer.m_alpha_frequency) + 1.0) * 0.5f * layer.m_alpha_amplitude) + layer.m_alpha_bias;
		break;
	case Shader::Layer::SQUARE:
		color.a = ((fmod(m_time + layer.m_alpha_phase, 1.0f/layer.m_alpha_frequency) > (1.0f/layer.m_alpha_frequency * layer.m_alpha_pwm)) ? 0.0f : layer.m_alpha_amplitude) + layer.m_alpha_bias;
		break;
	case Shader::Layer::TRIANGLE:
		alpha = (fmod(m_time + layer.m_alpha_phase, 1.0f/layer.m_alpha_frequency) / (1.0f/layer.m_alpha_frequency)) * 2.0f;
		color.a = ((alpha > 1.0f ? 2.0f - alpha : alpha) * layer.m_alpha_amplitude + layer.m_alpha_bias);
		break;
	case Shader::Layer::SAWTOOTH:
		color.a = fmod(m_time * layer.m_alpha_frequency * layer.m_alpha_amplitude + layer.m_alpha_bias,layer.m_alpha_amplitude);
		break;
	case Shader::Layer::INVERSE_SAWTOOTH:
		color.a = layer.m_alpha_amplitude - (fmod(m_time * layer.m_alpha_frequency + layer.m_alpha_bias,layer.m_alpha_amplitude));
		break;
	case Shader::Layer::NONE:
	default:
		break;
	}*/
	
	// *** set the color *******
	SetColor(color);

	// *** set envmap mode *******
/*
	switch(layer.m_uvgen)
	{
	case Shader::Layer::ENVMAP:
		glEnable(GL_TEXTURE_GEN_S);
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glEnable(GL_TEXTURE_GEN_T);
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		break;

	case Shader::Layer::NORMAL:
	default:
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);

		if (flags & LF_TEXCOORD_ARRAY)
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		else
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		break;
	}
*/
	// *** set depthtest *******
	if (layer.m_depth_test)
		EnableState(RenderingDevice::DEPTH_TEST);
	else
		DisableState(RenderingDevice::DEPTH_TEST);

	// *** set lighting ********
	if (layer.m_lighting)
		EnableState(RenderingDevice::LIGHTING);
	else
		DisableState(RenderingDevice::LIGHTING);

	// *** set linewidth *********
	m_line_width = layer.m_line_width;

	// *** uv transformation ********
/*
	m_texture_matrix = IdentityMatrix();

	for(std::list<Shader::Layer::TextureTransformation>::const_iterator i = layer.m_uv_transformations.begin(); i != layer.m_uv_transformations.end(); i++)
	{
		switch((*i).m_transform)
		{
		float scale,stretchx,stretchy;
		case Shader::Layer::SCROLL:
			scale = (*i).m_wave(m_time);
			glTranslatef(fmod((*i).m_direction.x() * scale,1.0f),fmod((*i).m_direction.y() * scale,1.0f),0.0f);
			break;
		case Shader::Layer::STRETCH:
			scale = (*i).m_wave(m_time);
			glTranslatef(0.5f,0.5f,0);
			stretchx = ((*i).m_direction.x() == 0.0f) ? 1.0 : (*i).m_direction.x() * scale;
			stretchy = ((*i).m_direction.y() == 0.0f) ? 1.0 : (*i).m_direction.y() * scale;
			glScalef(stretchx,stretchy,1.0f);
			glTranslatef(-0.5f,-0.5f,0);
			break;
		case Shader::Layer::ROTATE:
			scale = (*i).m_wave(m_time);
			glTranslatef(0.5f,0.5f,0);
			glRotatef(180.0f*scale/D_PI,0.0f,0.0f,1.0f);
			glTranslatef(-0.5f,-0.5f,0);
			break;
		default:
			break;
		}
	}
*/
}


void SoftwareDevice::Draw(HMesh mesh)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	D_ASSERT_MESSAGE(mesh.IsValid(), TEXT("Invalid mesh sent to device::Draw."));
	D_ASSERT_MESSAGE(mesh->Vertices() != 0, TEXT("You are trying to draw a mesh without any vertices."));
	D_ASSERT_MESSAGE(mesh->Indices() != 0, TEXT("You are trying to draw a mesh without any polygons."));
/*
	// these are not used yet
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);

	// gl is told where to find the vertices of the mesh
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, (void*)mesh->Vertices());

	int32 flags = 0;

	// set normal-array-pointer
	if (mesh->HasNormals())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, (void*)mesh->Normals());

		flags |= LF_NORMAL_ARRAY;
	}
	else
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	// set textcoordinates-array-pointer
	if (mesh->HasTexCoords())
	{
		glTexCoordPointer(2, GL_FLOAT, 0, (void*)mesh->TexCoords());
		flags |= LF_TEXCOORD_ARRAY;
	}
	
	int32 facecount = mesh->FaceCount();

	// *** vertex transformation ********


	std::list<Shader::VertexTransformation> &transforms = mesh->GetShader()->m_vertex_transformations;
	for(std::list<Shader::VertexTransformation>::iterator i = transforms.begin(); i != transforms.end();i++)
	{
		switch((*i).m_transform)
		{
		case Shader::VMOVE:
			glTranslatef((*i).m_wave[0](m_time), (*i).m_wave[1](m_time), (*i).m_wave[2](m_time));
			break;
		case Shader::VROTATE:
			glRotatef(180.0f*(*i).m_wave[2](m_time) / D_PI, 0,0,1);
			glRotatef(180.0f*(*i).m_wave[1](m_time) / D_PI, 0,1,0);
			glRotatef(180.0f*(*i).m_wave[0](m_time) / D_PI, 1,0,0);
			break;
		case Shader::VSCALE:
			glScalef((*i).m_wave[0](m_time), (*i).m_wave[1](m_time), (*i).m_wave[2](m_time));
			break;
		}
	}

	// draw all the passes
	const ListArray<Shader::Layer> &layers = mesh->GetShader()->m_layers;

	for (ListArray<Shader::Layer>::ConstIterator i = layers.Begin(); i.IsValid(); i++)
	{
		glPushMatrix();
		SetLayerState(*i, flags);
		glDrawElements(GL_TRIANGLES, facecount*3, GL_UNSIGNED_INT, (void*)mesh->Indices());
		glPopMatrix();
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	DisableState(LIGHTING);
	DisableState(BLEND);
	DisableState(CULL_FACE);
	SetDepthFunc(LESS);
	SetPolygonMode(FILL);
	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
*/
}

int32 SoftwareDevice::TexUnitCount()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	return 1;
}

void SoftwareDevice::ResetTexture()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_texture_matrix = IdentityMatrix();
	m_identity_texture = true;
}

void SoftwareDevice::TranslateTexture(const Vertex<3> &vertex)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_texture_matrix[0] += vertex.x();
	m_texture_matrix[5] += vertex.y();
	m_texture_matrix[10] += vertex.z();
}

void SoftwareDevice::RotateTexture(const Quaternion &rotation)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
/*
	glMatrixMode(GL_TEXTURE);
	Rotate(rotation);
	glMatrixMode(GL_MODELVIEW);
*/
}

void SoftwareDevice::PushMatrix()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	
	m_model_stack_ptr++;
	m_model_matrix[m_model_stack_ptr] = m_model_matrix[m_model_stack_ptr-1];
}

void SoftwareDevice::PopMatrix()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_model_stack_ptr--;
}

void SoftwareDevice::Begin(RenderingDevice::Primitive primitive)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	#ifdef _DEBUG
	m_in_begin = true;
	#endif
}

void SoftwareDevice::End()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	DrawCurrentFace();
	m_current_vertex = 0;

	#ifdef _DEBUG
	m_in_begin = false;
	#endif
}
void SoftwareDevice::AddVertex(const Point &vertex)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_vertices[m_current_vertex] = ModelMatrix() * Vertex<3>(vertex.x(), vertex.y(), m_near_z);
	m_vertices[m_current_vertex].x() /= -m_vertices[m_current_vertex].z();
	m_vertices[m_current_vertex].y() /= -m_vertices[m_current_vertex].z();
	m_vertices[m_current_vertex].z() = -m_vertices[m_current_vertex].z();
	m_current_vertex++;
}

void SoftwareDevice::AddVertex(const Vertex<3> &vertex)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	m_vertices[m_current_vertex] = ModelMatrix() * vertex;
	m_vertices[m_current_vertex].x() /= -m_vertices[m_current_vertex].z();
	m_vertices[m_current_vertex].y() /= -m_vertices[m_current_vertex].z();
	m_vertices[m_current_vertex].z() = -m_vertices[m_current_vertex].z();
	m_current_vertex++;
}

void SoftwareDevice::TexCoord(const Point &vertex)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	m_tex_coord[m_current_vertex] = vertex;
}

void SoftwareDevice::SetColor(const Color &color)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	m_color[m_current_vertex] = color;
}

void SoftwareDevice::SetColorMask(bool red, bool green, bool blue, bool alpha)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glColorMask(red, green, blue, alpha);
}

void SoftwareDevice::EnableState(RenderingDevice::RenderState state)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	if (state <= TEXTURE_2D) m_current_state.Enable(1<<state);
}

void SoftwareDevice::DisableState(RenderingDevice::RenderState state)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	if (state <= TEXTURE_2D) m_current_state.Disable(1<<state);
}

void SoftwareDevice::ClipAgainst(const Plane<3> &plane)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

}

void SoftwareDevice::Set2DMode(float width,float height)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);
*/
}


void SoftwareDevice::Set2DMode(const Rect &rectangle)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(rectangle.Left(), rectangle.Right(), rectangle.Bottom(), rectangle.Top());
	glMatrixMode(GL_MODELVIEW);
*/
}

void SoftwareDevice::Set3DMode(float vfov, float hfov, float near_clip, float far_clip)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum( -tan(hfov/2.0), tan(hfov/2.0), -tan(vfov/2.0), tan(vfov/2.0), near_clip,far_clip);
	glMatrixMode(GL_MODELVIEW);
*/
}

void SoftwareDevice::SetPerspective(float fov, float aspect, float near_clip, float far_clip)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, near_clip, far_clip);
	glMatrixMode(GL_MODELVIEW);
*/
}


void SoftwareDevice::SetPolygonMode(RenderingDevice::Fillmode f)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	m_current_state.SetFillMode(f);
}

void SoftwareDevice::SetBlendMode(RenderingDevice::Blendmode src, RenderingDevice::Blendmode dst)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_current_state.SetBlendMode(src, dst);
}

void SoftwareDevice::ClearDepthBuffer()
{
//	glClear(GL_DEPTH_BUFFER_BIT);
}

void SoftwareDevice::ClearColorBuffer()
{
	memset(m_buffer, *reinterpret_cast<int*>(&m_clear_color), m_width*m_height*4);
}

void SoftwareDevice::SetClearColor(const Color &clr)
{
	m_clear_color = clr;
}

void SoftwareDevice::SetDepthFunc(TestFunction func)
{
	m_current_state.SetDepthTestFunction(func);
}

void SoftwareDevice::SetCullMode(Faces mode)
{
	m_current_state.SetCullMode(mode);
}

Matrix4x4 SoftwareDevice::GetTextureMatrix() const
{
	return m_texture_matrix;
}

void SoftwareDevice::SetStates(const State &state)
{
	m_current_state.Set(state);

	D_ASSERT_MESSAGE(m_current_state.CurrentStates() == state.CurrentStates(), TEXT("Internal error, SoftwareDevice::SetStates() did not set the states correctly"));

	// set the right blendfunction if needed
	if (m_current_state.CurrentStates() & State::BLEND && (m_current_state.SourceBlend() != state.SourceBlend() || m_current_state.DestinationBlend() != state.DestinationBlend()))
	{
		m_current_state.SetBlendMode(state.SourceBlend(), state.DestinationBlend());
	}

	// set the right color if needed
	if (m_current_state.GetColor() != state.GetColor())
	{
		SetColor(state.GetColor());
//		m_current_state.SetColor(state.GetColor());
	}

	if (m_current_state.CurrentStates() & (State::TEXTURE_2D + State::TEXTURE_1D) && m_current_state.GetTexture() != state.GetTexture() && state.GetTexture().IsValid())
	{
		UseTexture(0, state.GetTexture());
//		m_current_state.SetTexture(state.GetTexture());
	}

	if (m_current_state.CurrentStates() & State::DEPTH_TEST && m_current_state.GetDepthTestFunction() != state.GetDepthTestFunction())
	{
		SetDepthFunc(state.GetDepthTestFunction());
//		m_current_state.SetDepthTestFunction(state.GetDepthTestFunction());
	}

	if (m_current_state.GetFillmode() != state.GetFillmode())
	{
		SetPolygonMode(state.GetFillmode());
//		m_current_state.SetFillMode(state.GetFillmode());
	}

	if (m_current_state.CurrentStates() & State::CULL_FACE && m_current_state.GetCullMode() != state.GetCullMode())
	{
		SetCullMode(state.GetCullMode());
	}

	if (m_current_state.ShadeModel() !=  state.ShadeModel())
	{
		SetShadeModel(state.ShadeModel());
	}
}

const State &SoftwareDevice::CurrentStates() const
{
	return m_current_state;
}

void SoftwareDevice::SetShadeModel(RenderingDevice::ShadeModel model)
{
	static const GLenum convert_table[] = 
	{
		GL_SMOOTH,
		GL_FLAT
	};

	glShadeModel(convert_table[model]);

	m_current_state.SetShadeModel(model);
}

void SoftwareDevice::Draw(const VertexArray &array)
{
/*
	// these are not used yet
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// gl is told where to find the vertices of the mesh
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, reinterpret_cast<void*>(*array.Vertices()));

	// set textcoordinates-array-pointer
	if (array.HasTextureCoords())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, reinterpret_cast<void*>(*array.TextureCoords()));
	}

	// set textcoordinates-array-pointer
	if (array.HasColors())
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, reinterpret_cast<void*>(*array.Colors()));
	}
	

	static const GLuint convert_table[] =
	{
		GL_POINTS,
		GL_LINES,
		GL_LINE_LOOP,
		GL_LINE_STRIP,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN,
		GL_QUADS,
		GL_QUAD_STRIP,
		GL_POLYGON
	};

	glDrawElements(convert_table[array.Primitive()], array.NumIndices(), GL_UNSIGNED_INT, reinterpret_cast<void*>(*array.Indices()));
//	glDrawArrays(GL_TRIANGLES,0,array.NumVertices());

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
*/
}

SoftwareDevice::SoftwareDevice(Window *window, const Rect &frame, uint32 flags, uint32 options, BView *view):
	m_window(window, this, frame, flags, options, view)
{
	logger(TEXT("Creating default texture\n"));

	char data[] = {0, 255, 255, 0};

	Bitmap null_bitmap(2, 2, Bitmap::GRAYSCALE);
	memcpy(null_bitmap.Data(), data, 4);
	
	Lock();
	m_null_texture = HTexture(null_bitmap, Texture::NO_MIPMAPS);
	UploadTexture(m_null_texture);
	Unlock();

	// Start timer
	m_timer.Start();

	#ifdef _DEBUG
	m_locked = false;
	m_in_begin = false;
	#endif
	
	m_texture_matrix = IdentityMatrix();
	m_identity_texture = true;
	m_model_stack_ptr = 0;
	m_model_matrix[0] = IdentityMatrix();
	m_identity_model = true;
	
	m_near_z = 1.0f;
	m_far_z = 1000.0f;
	
	m_clip_left = 0;
	m_clip_top = 0;
	m_clip_right = frame.width;
	m_clip_bottom = frame.height;
	
	m_current_vertex = 0;
}

SoftwareDevice::~SoftwareDevice()
{
}

WindowBase *SoftwareDevice::RenderWindow()
{
	return &m_window;
}

void SoftwareDevice::Lock()
{
	m_window.Lock();
	#ifdef _DEBUG
	m_locked = true;
	#endif
	m_buffer = reinterpret_cast<rgba_color*>(m_window.BackBuffer());
	m_width = m_window.Width() + 1;
	m_height = m_window.Height() + 1;
}

void SoftwareDevice::Unlock()
{
	m_window.Unlock();
	#ifdef _DEBUG
	m_locked = false;
	#endif
}

void SoftwareDevice::SwapBuffers()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_window.SwapBuffers();
	m_time = m_timer.Current();
}

void SoftwareDevice::SetFullScreen(bool fullscreen, int32 width, int32 height)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_window.SetFullScreen(fullscreen, width, height);
}

bool SoftwareDevice::IsFullScreen()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	return m_window.IsFullScreen();
}

bool SoftwareDevice::CanSetFullScreen()
{
	return true;
}

HTexture SoftwareDevice::NullTexture()
{
	return m_null_texture;
}





















void SoftwareDevice::DrawCurrentFace_()
{
	if (m_vertices[0].z() < m_near_z || m_vertices[1].z() < m_near_z || m_vertices[2].z() < m_near_z) return;

	int32 vx[3];
	int32 vy[3];
	int32 u[3];
	int32 v[3];

	for (int32 i = 0; i < 3; i++)
	{
		vx[i] = m_vertices[i].x() * 65536.0f;
		vy[i] = m_vertices[i].y();
		u[i] = m_tex_coord[i].u() * m_current_texture_width;
		v[i] = m_tex_coord[i].v() * m_current_texture_height;
	}

	//sort by y hardco(re)ded bub-sort
	if (vy[0] > vy[1]) { swap(vy[0],vy[1]); swap(vx[0],vx[1]); swap(u[0],u[1]); swap(v[0],v[1]);}
	if (vy[1] > vy[2]) { swap(vy[1],vy[2]); swap(vx[1],vx[2]); swap(u[1],u[2]); swap(v[1],v[2]);}
	if (vy[0] > vy[1]) { swap(vy[0],vy[1]); swap(vx[0],vx[1]); swap(u[0],u[1]); swap(v[0],v[1]);}

	if (vy[0] == vy[2]) return;

	int32 linestart,lineend;
	int32 add_start,add_end;
	int32 line_u_start,line_v_start;
	int32 u_line_add,v_line_add;
	int32 add_u,add_v;

	rgba_color *line_pointer = m_buffer + vy[0]	* m_width;
	line_u_start = u[0];
	line_v_start = v[0];

	int32 middle_x=vx[0]+((vx[2]-vx[0])/(vy[2]-vy[0])*(vy[1]-vy[0]));
	int32 middle_u=u[0]+((u[2]-u[0])/(vy[2]-vy[0])*(vy[1]-vy[0]));
	int32 middle_v=v[0]+((v[2]-v[0])/(vy[2]-vy[0])*(vy[1]-vy[0]));

	if (vx[1]>>16 == middle_x>>16) return;

	if (middle_x<vx[1])
	{
		add_u=(u[1]-middle_u)/(((vx[1]>>16)-(middle_x>>16)));
		add_v=(v[1]-middle_v)/(((vx[1]>>16)-(middle_x>>16)));
	}
	else
	{
		add_u=(middle_u-u[1])/(((middle_x>>16)-(vx[1]>>16)));
		add_v=(middle_v-v[1])/(((middle_x>>16)-(vx[1]>>16)));
	}

// ***** UPPER TRIANGLE *************
	
	if (vy[1] > vy[0])
	{
		linestart=lineend=vx[0];

		rgba_color *line_stop_pointer = m_buffer + vy[1] * m_width;
	
		if (middle_x < vx[1])
		{
			add_start=(vx[2]-linestart)/(vy[2]-vy[0]);
			add_end=(vx[1]-lineend)/(vy[1]-vy[0]);

			u_line_add=(u[2]-u[0])/(vy[2]-vy[0]);
			v_line_add=(v[2]-v[0])/(vy[2]-vy[0]);
		}
		else
		{
			add_start=(vx[1]-linestart)/(vy[1]-vy[0]);
			add_end=(vx[2]-lineend)/(vy[2]-vy[0]);

			u_line_add=(u[1]-u[0])/(vy[1]-vy[0]);
			v_line_add=(v[1]-v[0])/(vy[1]-vy[0]);
		}
	
	// CLIP Y

		if (vy[0] < m_clip_top)
		{
			line_pointer += (m_clip_top - vy[0]) * m_width;
			linestart	 += (m_clip_top - vy[0]) * add_start;
			lineend		 += (m_clip_top - vy[0]) * add_end;

			line_u_start += u_line_add * (m_clip_top-vy[0]);
			line_v_start += v_line_add * (m_clip_top-vy[0]);
		}

		if (vy[1]>m_clip_bottom + 1)
			line_stop_pointer = m_buffer + (m_clip_bottom + 1) * m_width;

		while (line_pointer<line_stop_pointer)
		{
			HorizLineTextured(line_pointer,linestart>>16,lineend>>16,m_current_texture,line_u_start,line_v_start,add_u,add_v);
			linestart += add_start;
			lineend	 += add_end;
			line_pointer += m_width;
			
			line_u_start += u_line_add;
			line_v_start += v_line_add;
		}
	}

// ***** BOTTOM TRIANGLE *************

	if (vy[2] > vy[1])
	{
		rgba_color *line_stop_pointer = m_buffer + vy[2] * m_width;

		if (middle_x<vx[1])
		{
			linestart=middle_x;
			lineend=vx[1];

			line_u_start = middle_u;
			line_v_start = middle_v;

			add_start=(vx[2]-vx[0])/(vy[2]-vy[0]);
			add_end=(vx[2]-vx[1])/(vy[2]-vy[1]);

			u_line_add=(u[2]-u[0])/(vy[2]-vy[0]);
			v_line_add=(v[2]-v[0])/(vy[2]-vy[0]);
		}
		else
		{
			linestart=vx[1];
			lineend=middle_x;

			line_u_start = u[1];
			line_v_start = v[1];

			add_start=(vx[2]-vx[1])/(vy[2]-vy[1]);
			add_end=(vx[2]-vx[0])/(vy[2]-vy[0]);

			u_line_add=(u[2]-u[1])/(vy[2]-vy[1]);
			v_line_add=(v[2]-v[1])/(vy[2]-vy[1]);
		}
	
	// CLIP Y

		if (vy[1] < m_clip_top)
		{
			line_pointer = m_buffer + m_clip_top * m_width;
			linestart	 += (m_clip_top - vy[1]) * add_start;
			lineend		 += (m_clip_top - vy[1]) * add_end;

			line_u_start += u_line_add * (m_clip_top-vy[1]);
			line_v_start += v_line_add * (m_clip_top-vy[1]);
		}

		if (vy[2]>m_clip_bottom + 1)
			line_stop_pointer = m_buffer + (m_clip_bottom + 1) * m_width;

		while (line_pointer<line_stop_pointer)
		{
			HorizLineTextured(line_pointer,linestart>>16,lineend>>16,m_current_texture,line_u_start,line_v_start,add_u,add_v);
			linestart += add_start;
			lineend	 += add_end;
			line_pointer += m_width;
			
			line_u_start += u_line_add;
			line_v_start += v_line_add;
		}
	}	
}


void SoftwareDevice::DrawCurrentFace()
{
	if (m_vertices[0].z() < m_near_z || m_vertices[1].z() < m_near_z || m_vertices[2].z() < m_near_z) return;

	int32 vx[3];
	int32 vy[3];
	int32 u[3];
	int32 v[3];

	for (int32 i = 0; i < 3; i++)
	{
		vx[i] = m_vertices[i].x() * 65536.0f;
		vy[i] = m_vertices[i].y();
		u[i] = m_tex_coord[i].u() * m_current_texture_width;
		v[i] = m_tex_coord[i].v() * m_current_texture_height;
	}

	//sort by y hardco(re)ded bub-sort
	if (vy[0] > vy[1]) { swap(vy[0],vy[1]); swap(vx[0],vx[1]); swap(u[0],u[1]); swap(v[0],v[1]);}
	if (vy[1] > vy[2]) { swap(vy[1],vy[2]); swap(vx[1],vx[2]); swap(u[1],u[2]); swap(v[1],v[2]);}
	if (vy[0] > vy[1]) { swap(vy[0],vy[1]); swap(vx[0],vx[1]); swap(u[0],u[1]); swap(v[0],v[1]);}

	if (vy[0] == vy[2]) return;

	int32 linestart,lineend;
	int32 add_start,add_end;
	int32 line_u_start,line_v_start;
	int32 u_line_add,v_line_add;
	int32 add_u,add_v;

	rgba_color *line_pointer = m_buffer + vy[0]	* m_width;
	line_u_start = u[0];
	line_v_start = v[0];

	int32 middle_x=vx[0]+((vx[2]-vx[0])/(vy[2]-vy[0])*(vy[1]-vy[0]));
	int32 middle_u=u[0]+((u[2]-u[0])/(vy[2]-vy[0])*(vy[1]-vy[0]));
	int32 middle_v=v[0]+((v[2]-v[0])/(vy[2]-vy[0])*(vy[1]-vy[0]));

	if (vx[1]>>16 == middle_x>>16) return;

	if (middle_x<vx[1])
	{
		add_u=(u[1]-middle_u)/(((vx[1]>>16)-(middle_x>>16)));
		add_v=(v[1]-middle_v)/(((vx[1]>>16)-(middle_x>>16)));
	}
	else
	{
		add_u=(middle_u-u[1])/(((middle_x>>16)-(vx[1]>>16)));
		add_v=(middle_v-v[1])/(((middle_x>>16)-(vx[1]>>16)));
	}

// ***** UPPER TRIANGLE *************
	
	if (vy[1] > vy[0])
	{
		linestart=lineend=vx[0];

		rgba_color *line_stop_pointer = m_buffer + vy[1] * m_width;
	
		if (middle_x < vx[1])
		{
			add_start=(vx[2]-linestart)/(vy[2]-vy[0]);
			add_end=(vx[1]-lineend)/(vy[1]-vy[0]);

			u_line_add=(u[2]-u[0])/(vy[2]-vy[0]);
			v_line_add=(v[2]-v[0])/(vy[2]-vy[0]);
		}
		else
		{
			add_start=(vx[1]-linestart)/(vy[1]-vy[0]);
			add_end=(vx[2]-lineend)/(vy[2]-vy[0]);

			u_line_add=(u[1]-u[0])/(vy[1]-vy[0]);
			v_line_add=(v[1]-v[0])/(vy[1]-vy[0]);
		}
	
	// CLIP Y

		if (vy[0] < m_clip_top)
		{
			line_pointer += (m_clip_top - vy[0]) * m_width;
			linestart	 += (m_clip_top - vy[0]) * add_start;
			lineend		 += (m_clip_top - vy[0]) * add_end;

			line_u_start += u_line_add * (m_clip_top-vy[0]);
			line_v_start += v_line_add * (m_clip_top-vy[0]);
		}

		if (vy[1]>m_clip_bottom + 1)
			line_stop_pointer = m_buffer + (m_clip_bottom + 1) * m_width;

		while (line_pointer<line_stop_pointer)
		{
			HorizLineFlat(line_pointer,linestart>>16,lineend>>16,m_color[0]);
			linestart += add_start;
			lineend	 += add_end;
			line_pointer += m_width;
			
			line_u_start += u_line_add;
			line_v_start += v_line_add;
		}
	}

// ***** BOTTOM TRIANGLE *************

	if (vy[2] > vy[1])
	{
		rgba_color *line_stop_pointer = m_buffer + vy[2] * m_width;

		if (middle_x<vx[1])
		{
			linestart=middle_x;
			lineend=vx[1];

			line_u_start = middle_u;
			line_v_start = middle_v;

			add_start=(vx[2]-vx[0])/(vy[2]-vy[0]);
			add_end=(vx[2]-vx[1])/(vy[2]-vy[1]);

			u_line_add=(u[2]-u[0])/(vy[2]-vy[0]);
			v_line_add=(v[2]-v[0])/(vy[2]-vy[0]);
		}
		else
		{
			linestart=vx[1];
			lineend=middle_x;

			line_u_start = u[1];
			line_v_start = v[1];

			add_start=(vx[2]-vx[1])/(vy[2]-vy[1]);
			add_end=(vx[2]-vx[0])/(vy[2]-vy[0]);

			u_line_add=(u[2]-u[1])/(vy[2]-vy[1]);
			v_line_add=(v[2]-v[1])/(vy[2]-vy[1]);
		}
	
	// CLIP Y

		if (vy[1] < m_clip_top)
		{
			line_pointer = m_buffer + m_clip_top * m_width;
			linestart	 += (m_clip_top - vy[1]) * add_start;
			lineend		 += (m_clip_top - vy[1]) * add_end;

			line_u_start += u_line_add * (m_clip_top-vy[1]);
			line_v_start += v_line_add * (m_clip_top-vy[1]);
		}

		if (vy[2]>m_clip_bottom + 1)
			line_stop_pointer = m_buffer + (m_clip_bottom + 1) * m_width;

		while (line_pointer<line_stop_pointer)
		{
			HorizLineFlat(line_pointer,linestart>>16,lineend>>16,m_color[0]);
			linestart += add_start;
			lineend	 += add_end;
			line_pointer += m_width;
			
			line_u_start += u_line_add;
			line_v_start += v_line_add;
		}
	}	
}
