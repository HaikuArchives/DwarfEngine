
/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

/*
 * OpenGLDevice_common_code.h
 *
 * This file contains the opengl code that is common to all
 * platforms. It's primary function is to make it easier to
 * maintain all platforms (so we don't have to cut'n'paste).
 *
 */
 

// checks to see if x = 2^y
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

void OpenGLDevice::Viewport(const Rect &rect)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	// set states
//	logger(TEXT("Setting gl viewport to (%f,%f,%f,%f)"), r.x, r.y, r.width, r.height);
	glViewport( rect.x, rect.y, rect.width, rect.height);
}

void OpenGLDevice::Scissor(const Rect &rect)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glScissor(static_cast<int32>(rect.x), static_cast<int32>(rect.y), static_cast<int32>(rect.width), static_cast<int32>(rect.height));
}

void OpenGLDevice::LoadIdentity()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glLoadIdentity();
}

void OpenGLDevice::Translate(const Vertex<3> &point)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glTranslatef(point.x(), point.y(), point.z());
}

void OpenGLDevice::Rotate(const Quaternion &rotation)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
//	Vertex<3> axis;
//	float angle;
//	rotation.ToAxis(&angle, &axis);
//	glRotatef(RadToDeg(angle), axis.x(), axis.y(), axis.z());
	Matrix4x4 m = rotation.ToMatrix();
	glMultMatrixf(m);
}

void OpenGLDevice::MultMatrix(const Matrix4x4 &matrix)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	Matrix4x4 m = matrix;
	glMultMatrixf(m);
}

void OpenGLDevice::Scale(const Vertex<3> &factor)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glScalef(factor.x(), factor.y(), factor.z());
}

void OpenGLDevice::SetTextureFilters(HTexture texture)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(texture.IsValid(), TEXT("You are using an uninitialized texture."));
	const int32 correction[2] =
	{
		GL_FASTEST,
		GL_NICEST
	};

	const int32 apply[4] =
	{
		GL_REPLACE,
		GL_DECAL,
		GL_MODULATE,
		GL_BLEND
	};

	const int32 filter[2] =
	{
		GL_NEAREST,
		GL_LINEAR
	};

	const int32 mipmap[5] =
	{
		GL_NEAREST,  // NO_MIPMAPS
		GL_NEAREST_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_LINEAR
	};

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

}

void OpenGLDevice::UseTexture(int32 texture_unit, HTexture texture) // sets the given texture, uploading it if it isn't on the card
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(texture.IsValid(), TEXT("Invalid texture given to OpenGLDevice::UseTexture."));
	D_ASSERT_MESSAGE(!m_in_begin, TEXT("You cannot change texture between Begin() and End()"));

	if (!texture->IsResident())
	{
		UploadTexture(texture);
	}

	m_current_state.SetTexture(texture);
//	logger(TEXT("UseTexture: %3d TexUnit: %3d"), texture->Index(), texture_unit);

	if (texture_unit < 1)
	{
		glBindTexture(GL_TEXTURE_2D, texture->Index());
//		SetTextureFilters(texture);
	}
	else
	{
		D_ASSERT_MESSAGE(texture_unit < 1, TEXT("Multitexturing not yet supported."));
	}
}

void OpenGLDevice::UploadTexture(HTexture texture) // uploads the given texture to the card
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(texture.IsValid(), TEXT("Invalid handle given to UploadTexture!"));


	if (texture->IsResident())
	{
		RemoveTexture(texture);
	}

	D_ASSERT_MESSAGE(texture->GetBitmap(), TEXT("UploadTexture() called on texture with no bitmap!"));

	Bitmap& bitmap = *texture->GetBitmap();

//	D_ASSERT_MESSAGE(israised2(texture->Width()) && israised2(texture->Height()), 
//		TEXT("Bitmap dimensions are not multiples of two!"));

	// allocate names for all the texture
	GLuint tex_name;
	glGenTextures(1, &tex_name);

	// bind the graphics to the texture
	glBindTexture(GL_TEXTURE_2D, tex_name);

	SetTextureFilters(texture);

	static const int32 convert[] =
	{
		GL_STENCIL_INDEX,
		GL_DEPTH_COMPONENT,
		GL_RED,
		GL_GREEN,
		GL_BLUE,
		GL_ALPHA,
		GL_RGB,
		GL_RGBA,
		GL_LUMINANCE,
		GL_LUMINANCE_ALPHA //,
//		GL_BGR,
//		GL_BGRA
	};

    if ( texture->MipmapMode() == Texture::NO_MIPMAPS )
    {
		glTexImage2D(GL_TEXTURE_2D,0,
			bitmap.BytesPerPixel(),
			texture->Width(), texture->Height(), 0,
			convert[bitmap.Space()],
			GL_UNSIGNED_BYTE, bitmap.Data());
	}
	else
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D,
			bitmap.BytesPerPixel(),
			texture->Width(), texture->Height(),
			convert[bitmap.Space()],
			GL_UNSIGNED_BYTE, bitmap.Data());
	}

	texture->SetIndex(tex_name);
	
	logger(TEXT("Texture index %d assigned to (%dx%d, %d)"), 
		texture->Index(), bitmap.Width(), bitmap.Height(), bitmap.BytesPerPixel());
}

void OpenGLDevice::RemoveTexture(HTexture texture) // removes the given texture from the card
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(texture.IsValid(), TEXT("Invalid handle given to RemoveTexture!"));
	if (!texture->IsResident())
		return;			// texture was non resident (does not reside on the card)
	
	GLuint textures = texture->Index();

	texture->SetIndex(-1);

	glDeleteTextures(1, &textures);
}

void OpenGLDevice::SetDepthRange(float min, float max)
{
	glDepthRange(min, max);
}


void OpenGLDevice::SetMaterialState(HMaterial material)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	D_ASSERT_MESSAGE(material.IsValid(), TEXT("Invalid material given to SetMaterialState"));

	uint32 flags = material->GetFlags();

	if (flags & Material::M_TRANSPARENT)
	{
		EnableState(RenderingDevice::BLEND);
	}
	else
	{
		DisableState(RenderingDevice::BLEND);
	}

	EnableState(RenderingDevice::DEPTH_TEST);

	SetColor(material->Diffuse());
}

void OpenGLDevice::CreateLight(int32 index, Vertex<3> const& pos, Color const& color)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	GLuint indarr[] =
	{
		GL_LIGHT0,
		GL_LIGHT1,
		GL_LIGHT2,
		GL_LIGHT3,
		GL_LIGHT4,
		GL_LIGHT5,
		GL_LIGHT6,
		GL_LIGHT7
	};

	glEnable(indarr[index]);
	glLightfv(indarr[index], GL_DIFFUSE, color);
	glLightfv(indarr[index], GL_POSITION, pos);
}

void OpenGLDevice::DisableLight(int32 index)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	GLuint indarr[] =
	{
		GL_LIGHT0,
		GL_LIGHT1,
		GL_LIGHT2,
		GL_LIGHT3,
		GL_LIGHT4,
		GL_LIGHT5,
		GL_LIGHT6,
		GL_LIGHT7
	};

	glDisable(indarr[index]);
}



enum LayerFlags
{
	LF_TEXCOORD_ARRAY	= 0x00000001,
	LF_NORMAL_ARRAY		= 0x00000002,
	LF_COLOR_ARRA		= 0x00000004,
	LF_INDEX_ARRAY		= 0x00000008,
	LF_EDGEFLAG_ARRAY	= 0x00000010
};

void OpenGLDevice::SetClipPlane(const Plane<3> &plane)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	static double equation[4];
	equation[0] = plane.Normal().x();
	equation[1] = plane.Normal().y();
	equation[2] = plane.Normal().z();
	equation[3] = plane.D();

	glClipPlane(GL_CLIP_PLANE0, equation);
}

void OpenGLDevice::SetLayerState(const Shader::Layer &layer, int32 flags)
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
	glLineWidth(layer.m_line_width);

	// *** uv transformation ********

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

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
	glMatrixMode(GL_MODELVIEW);
}


void OpenGLDevice::Draw(HMesh mesh)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	D_ASSERT_MESSAGE(mesh.IsValid(), TEXT("Invalid mesh sent to device::Draw."));
	D_ASSERT_MESSAGE(mesh->Vertices() != 0, TEXT("You are trying to draw a mesh without any vertices."));
	D_ASSERT_MESSAGE(mesh->Indices() != 0, TEXT("You are trying to draw a mesh without any polygons."));

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
}

int32 OpenGLDevice::TexUnitCount()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	return 1;
}

void OpenGLDevice::ResetTexture()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

void OpenGLDevice::TranslateTexture(const Vertex<3> &vertex)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	glMatrixMode(GL_TEXTURE);
	Translate(vertex);
	glMatrixMode(GL_MODELVIEW);
}

void OpenGLDevice::RotateTexture(const Quaternion &rotation)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	glMatrixMode(GL_TEXTURE);
	Rotate(rotation);
	glMatrixMode(GL_MODELVIEW);
}

void OpenGLDevice::PushMatrix()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glPushMatrix();
}

void OpenGLDevice::PopMatrix()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glPopMatrix();
}

void OpenGLDevice::Begin(RenderingDevice::Primitive primitive)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	#ifdef _DEBUG
	m_in_begin = true;
	#endif
	glBegin(primitive);
}

void OpenGLDevice::End()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	glEnd();
	#ifdef _DEBUG
	m_in_begin = false;
	#endif
}
void OpenGLDevice::AddVertex(const Point &vertex)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glVertex2fv(vertex);
}

void OpenGLDevice::AddVertex(const Vertex<3> &vertex)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glVertex3fv(vertex);
}

void OpenGLDevice::TexCoord(const Point &vertex)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glTexCoord2fv(vertex);
}

void OpenGLDevice::SetColor(const Color &color)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glColor4fv(color);
}

void OpenGLDevice::SetColorMask(bool red, bool green, bool blue, bool alpha)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glColorMask(red, green, blue, alpha);
}

void OpenGLDevice::EnableState(RenderingDevice::RenderState state)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	static int32 convert[] =
	{
		GL_ALPHA_TEST,
		GL_AUTO_NORMAL,
		GL_BLEND,

		GL_COLOR_MATERIAL,
		GL_CULL_FACE,
		GL_DEPTH_TEST,
		GL_DITHER,

		GL_FOG,

		GL_LIGHTING,
		GL_LINE_SMOOTH,
		GL_LINE_STIPPLE,
		GL_LOGIC_OP,

		GL_POINT_SMOOTH,
		GL_POLYGON_SMOOTH,
		GL_POLYGON_STIPPLE,
		GL_SCISSOR_TEST,
		GL_STENCIL_TEST,
		GL_TEXTURE_1D,
		GL_TEXTURE_2D,

		GL_LIGHT0,
		GL_LIGHT1,
		GL_LIGHT2,
		GL_LIGHT3,
		GL_LIGHT4,
		GL_LIGHT5,
		GL_LIGHT6,
		GL_LIGHT7,

		GL_CLIP_PLANE0,
		GL_CLIP_PLANE1,
		GL_CLIP_PLANE2,
		GL_CLIP_PLANE3,
		GL_CLIP_PLANE4,
		GL_CLIP_PLANE5
	};

	if (state <= TEXTURE_2D) m_current_state.Enable(1<<state);
	glEnable(convert[state]);
}

void OpenGLDevice::DisableState(RenderingDevice::RenderState state)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	static int32 convert[] =
	{
		GL_ALPHA_TEST,
		GL_AUTO_NORMAL,
		GL_BLEND,

		GL_COLOR_MATERIAL,
		GL_CULL_FACE,
		GL_DEPTH_TEST,
		GL_DITHER,

		GL_FOG,

		GL_LIGHTING,
		GL_LINE_SMOOTH,
		GL_LINE_STIPPLE,
		GL_LOGIC_OP,

		GL_POINT_SMOOTH,
		GL_POLYGON_SMOOTH,
		GL_POLYGON_STIPPLE,
		GL_SCISSOR_TEST,
		GL_STENCIL_TEST,
		GL_TEXTURE_1D,
		GL_TEXTURE_2D,

		GL_LIGHT0,
		GL_LIGHT1,
		GL_LIGHT2,
		GL_LIGHT3,
		GL_LIGHT4,
		GL_LIGHT5,
		GL_LIGHT6,
		GL_LIGHT7,

		GL_CLIP_PLANE0,
		GL_CLIP_PLANE1,
		GL_CLIP_PLANE2,
		GL_CLIP_PLANE3,
		GL_CLIP_PLANE4,
		GL_CLIP_PLANE5
	};

	if (state <= TEXTURE_2D) m_current_state.Disable(1<<state);
	glDisable(convert[state]);
}

void OpenGLDevice::ClipAgainst(const Plane<3> &plane)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
//	glClipPlane();
}

void OpenGLDevice::Set2DMode(float width,float height)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);
}


void OpenGLDevice::Set2DMode(const Rect &rectangle)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(rectangle.Left(), rectangle.Right(), rectangle.Bottom(), rectangle.Top());
	glMatrixMode(GL_MODELVIEW);
}

void OpenGLDevice::Set3DMode(float vfov, float hfov, float near_clip, float far_clip)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum( -tan(hfov/2.0), tan(hfov/2.0), -tan(vfov/2.0), tan(vfov/2.0), near_clip,far_clip);
	glMatrixMode(GL_MODELVIEW);
}

void OpenGLDevice::SetPerspective(float fov, float aspect, float near_clip, float far_clip)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, near_clip, far_clip);
	glMatrixMode(GL_MODELVIEW);
}


void OpenGLDevice::SetPolygonMode(RenderingDevice::Fillmode f)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	static int32 convert[] =
	{
		GL_POINT,
		GL_LINE,
		GL_FILL
	};

	m_current_state.SetFillMode(f);
	glPolygonMode(GL_FRONT_AND_BACK, convert[f]);
}

void OpenGLDevice::SetBlendMode(RenderingDevice::Blendmode src, RenderingDevice::Blendmode dst)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));
	static int32 convert[] =
	{
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA_SATURATE
	};

	m_current_state.SetBlendMode(src, dst);
	glBlendFunc(convert[src], convert[dst]);
}

void OpenGLDevice::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLDevice::ClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLDevice::SetClearColor(const Color &clr)
{
	glClearColor(clr.r,clr.g,clr.b,clr.a);
}

void OpenGLDevice::SetDepthFunc(TestFunction func)
{
	static int32 convert[] = {	GL_NEVER,GL_LESS,GL_EQUAL,GL_LEQUAL,
								GL_GREATER,GL_NOTEQUAL,GL_GEQUAL,GL_ALWAYS };
	m_current_state.SetDepthTestFunction(func);
	glDepthFunc(convert[func]);
}

void OpenGLDevice::SetCullMode(Faces mode)
{
	static int32 convert[] = {GL_FRONT,GL_BACK,FRONT_AND_BACK };

	m_current_state.SetCullMode(mode);
	glCullFace(convert[mode]);
}

Matrix4x4 OpenGLDevice::GetTextureMatrix() const
{
	Matrix4x4 m;
	glGetFloatv(GL_TEXTURE_MATRIX, m);

	return m;
}

void OpenGLDevice::SetStates(const State &state)
{
	static const int32 state_convert[] =
	{
		GL_ALPHA_TEST,
		GL_AUTO_NORMAL,
		GL_BLEND,

		GL_COLOR_MATERIAL,
		GL_CULL_FACE,
		GL_DEPTH_TEST,
		GL_DITHER,

		GL_FOG,

		GL_LIGHTING,
		GL_LINE_SMOOTH,
		GL_LINE_STIPPLE,
		GL_LOGIC_OP,

		GL_POINT_SMOOTH,
		GL_POLYGON_SMOOTH,
		GL_POLYGON_STIPPLE,
		GL_SCISSOR_TEST,
		GL_STENCIL_TEST,
		GL_TEXTURE_1D,
		GL_TEXTURE_2D,
	};

	static int32 blend_convert[] =
	{
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA_SATURATE
	};
	

	uint32 current_state = m_current_state.CurrentStates();
	uint32 changes = (current_state ^ state.CurrentStates()) & state.UsedStates();
	
	// only proceed if there are any state changes
	if (changes != 0)
	{
		for (int32 i = 0; i <= TEXTURE_2D; i++)
		{
			const uint32 bit = 1 << i;
			if ((changes & bit) == 0) continue;
			
			if (state.CurrentStates() & bit)
				glEnable(state_convert[i]);
			else
				glDisable(state_convert[i]);
		}
	}

	m_current_state.Set(state);

	D_ASSERT_MESSAGE(m_current_state.CurrentStates() == state.CurrentStates(), TEXT("Internal error, OpenGLDevice::SetStates() did not set the states correctly"));

	// set the right blendfunction if needed
	if (m_current_state.CurrentStates() & State::BLEND && (m_current_state.SourceBlend() != state.SourceBlend() || m_current_state.DestinationBlend() != state.DestinationBlend()))
	{
		glBlendFunc(blend_convert[state.SourceBlend()], blend_convert[state.DestinationBlend()]);
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

const State &OpenGLDevice::CurrentStates() const
{
	return m_current_state;
}

void OpenGLDevice::SetShadeModel(RenderingDevice::ShadeModel model)
{
	static const GLenum convert_table[] = 
	{
		GL_SMOOTH,
		GL_FLAT
	};

	glShadeModel(convert_table[model]);

	m_current_state.SetShadeModel(model);
}

void OpenGLDevice::Draw(const VertexArray &array)
{
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
}
