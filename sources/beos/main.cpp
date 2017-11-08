/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include <Message.h>
#include <Application.h>
#include <Alert.h>
#include <Roster.h>
#include <Path.h>
#include <Entry.h>
#include <dirent.h>
#include <image.h>
#include <unistd.h>

#include "String.h"
#include "Array.h"
#include "FileSystem.h"
#include "Exception.h"
#include "ImageTranslator.h"
#include "Directory.h"
#include "File.h"
#include "Log.h"
#include "Codec3DS.h"
#include "LwobCodec.h"

#include "OpenGLDevice.h"
#include "SoftwareDevice.h"


using namespace dwarf;


extern int32 DwarfMain(Array<String> &commandline);


	class Application : public BApplication
	{
		public:

		Application(const char *signature):
			BApplication(signature)
		{}

		virtual ~Application()
		{}

		virtual void ArgvReceived(int32 argc, char **argv)
		{
			m_argc = argc;
			m_argv = argv;
		}

		virtual void ReadyToRun()
		{
			m_thread = spawn_thread(main_thread, "main_thread", B_DISPLAY_PRIORITY, this);
			resume_thread(m_thread);
		}
	
		static int32 main_thread(void *param)
		{
			Application *app = reinterpret_cast<Application*>(param);
			Array<String> command(app->m_argc);
			for (int32 i = 0; i < app->m_argc; i++)
				command[i] = app->m_argv[i];


		#ifdef _DEBUG
			logger.SetLogLevel(Log::HIGH);
		#else
			logger.SetLogLevel(Log::LOW);
		#endif

			Directory log_dir;
			File log_file(&log_dir, TEXT("dwarf.log"), File::WRITE, File::CREATE | File::TEXT);
			logger.PushOutput(&log_file);


			ImageTranslator translator;
			Codec3DS codec3ds;
			LwobCodec lwob_codec;

			app->m_return_value = DwarfMain(command);
			
		#ifdef _DEBUG
			m_dumpMemoryReport("memreport.log", true);
		#endif
			
			app->PostMessage(B_QUIT_REQUESTED);

			logger.PopOutput();

			return 0;
		}

		int32 ReturnValue()
		{
			return m_return_value;
		}

		private:

		thread_id m_thread;	// the main thread's id
		int m_argc;				// the arg count
		char **m_argv;			// the arguments list
		int32 m_return_value;	// the return value of the application
	};



int main()
{
	try
	{
		Application app("application/x-vnd.Arvid_Norberg-Kristoffer_Grönlund-DwarfEngine");
		app.Run();
		return app.ReturnValue();
	}
	catch (const Exception &e)
	{
		e.OpenDialog();
		return 1;
	}
}

#include "nommgr.h"

namespace dwarf
{
	RenderingDevice *create_default_device(Window* window, const Rect &frame, uint32 flags, uint32 options)
	{
		class __ResizeView : public BView
		{
		public:
			__ResizeView(dwarf::Window &window):BView(BRect(0,0,10,10), "resize_view", B_FOLLOW_ALL_SIDES, B_FRAME_EVENTS + B_WILL_DRAW + B_NAVIGABLE),
				m_window(window) {}
			void *operator new(size_t size) { return malloc(size); }
			void operator delete(void *ptr) { free(ptr); }
			void AttachedToWindow() { BRect r = Window()->Bounds(); MoveTo(0,0); ResizeTo(r.Width(), r.Height()); }
			void FrameResized(float width, float height) { BView::FrameResized(width,height); m_window.OnResize(width, height); }
		private:
			dwarf::Window &m_window;
		};

//		return new OpenGLDevice(window, frame, flags, options, new __ResizeView(*window));
		return new SoftwareDevice(window, frame, flags, options, new __ResizeView(*window));
	}

	void remove_default_device(RenderingDevice *device)
	{
		delete device;
	}

	void sleep(float seconds)
	{
		snooze(static_cast<bigtime_t>(seconds * 1000000.0f));
	}

	/*!
		The functions doesn't return until the user has dismissed the dialog.
		\brief Opens a messagebox.
		\param title The message box' title
		\param body The message
	*/
	void MsgBox(const String &title, const String &body)
	{
		char c_message[512];
		char c_title[50];
		(new BAlert(title.ToChar(c_title, 50),body.ToChar(c_message, 512),"ok"))->Go();
	}

	String FileSystem::BasePath()
	{
		app_info info;
		be_app->GetAppInfo(&info);
		BEntry entry(&info.ref);
		BPath path;
		entry.GetPath(&path);
		path.GetParent(&path);
	
		String base_path(path.Path());
		base_path += TEXT("/");
		return base_path;
	}

	void enter_debugger()
	{
		debugger("");
	}
	
	void assert_message(const char *file, uint32 line, const dchar *condition, const dchar *message)
	{
		dwarf::String msg;
#ifdef D_UNICODE
		msg.Format(TEXT("Assert Failed!\n\n\nIn file: %S:%d\nThe statement that failed was '%s': %s\n"), file, line, condition, message);
#else	
		msg.Format(TEXT("Assert Failed!\n\n\nIn file: %s:%d\nThe statement that failed was '%s': %s\n"), file, line, condition, message);
#endif
		logger(*msg);
		char buffer[1024];
		msg.ToChar(buffer, 1024);
		::debugger(buffer);
	}
}
