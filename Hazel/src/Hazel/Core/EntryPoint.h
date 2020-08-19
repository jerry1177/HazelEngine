#pragma once
#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();
int main(int argc, char** argv ) {
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized");
	HZ_PROFILE_BEGIN_SESSION("Start up session", "HazelProfile-startup.json");
	int a = 5;
	HZ_INFO("Hello from client! var={0}", a);
	auto app = Hazel::CreateApplication();
	HZ_PROFILE_END_SESSION();
	HZ_PROFILE_BEGIN_SESSION("Run Time session", "HazelProfile-runtime.json");
	app->Run();
	HZ_PROFILE_END_SESSION();
	HZ_PROFILE_BEGIN_SESSION("End Time session", "HazelProfile-endtime.json");
	delete app;
	HZ_PROFILE_END_SESSION();
}
	
#endif // HZ_PLATFORM_WINDOWS
