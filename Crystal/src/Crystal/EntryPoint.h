#pragma once

#ifdef CRYSTAL_PLATFORM_WINDOWS

extern Crystal::Application* Crystal::CreateApplication();

int main(int argc, char** argv)
{
	Crystal::Log::Init();
	CRYSTAL_CORE_WARN("Initialized Log!");
	CRYSTAL_CLIENT_INFO("Hello!");

	auto app = Crystal::CreateApplication();
	app->Run();
	delete app;
}

#endif
