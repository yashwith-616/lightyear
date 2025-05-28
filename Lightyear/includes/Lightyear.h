#pragma once

#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Layer.h"
#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/Events/Event.h"

#pragma region Renderer
#include "Lightyear/Renderer/Abstract/RenderCommand.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

#include "Lightyear/Renderer/Primitives/Buffer.h"
#include "Lightyear/Renderer/Primitives/Framebuffer.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"
#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Renderer/Primitives/Texture.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

// Camera
#include "Lightyear/Renderer/Camera/Camera.h"
#include "Lightyear/Renderer/Camera/OrthographicCamera.h"
#include "Lightyear/Renderer/Camera/SceneCamera.h"

#pragma endregion

#pragma region Scene
#include "Lightyear/Scene/Components.h"
#include "Lightyear/Scene/Entity.h"
#include "Lightyear/Scene/Scene.h"

#pragma endregion

// Input
#include "Lightyear/Input/Input.h"