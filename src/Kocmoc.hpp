#include <common.hpp>
#include <time/Timer.hpp>
#include <camera/OrthoCamera.hpp>

namespace kocmoc
{
	namespace camera
	{
		class FilmCamera;
		class Camera;
	}

	namespace time
	{
		class Clock;
	}

	namespace scene
	{
		class PolyMeshNode;
	}

	namespace renderer
	{
		class Shader;
		class FrameBuffer;
		class ShadowMap;
	}

	namespace input
	{
		class Gamepad;
	}

	/**
	 * Singleton game object. All game related magic happens in here.
	 */
	class Kocmoc
	{
	public:

		/**
		 * Get the instance of the singleton
		 */
		static Kocmoc& getInstance();

		static void Destroy();

		/**
		 * Kickstart the action!
		 */
		void start();

		void stop();


		/**
		 * draw the current frame
		 */
		void draw(void);

		/**
		 * init everything that is needed for the game
		 */
		void init();

		bool isRunning();

		camera::FilmCamera* getCamera(void) {return camera;};
		camera::Camera *getOrthoCamera(void) {return orthoCamera;};

		/**
		 * reload some stuff
		 */
		void reload(void);

		/**
		 * Get the main clock 
		 */
		time::Clock* getClock(void) {return clock;};
	
	private:

		Kocmoc();
		Kocmoc(const Kocmoc& cc); // protect the copy constructor as well
		~Kocmoc();

		static Kocmoc* instance;

		bool running;
		
		time::Timer timer;

		camera::FilmCamera* camera;
		camera::OrthoCamera* orthoCamera;

		scene::PolyMeshNode* rootNode;
		scene::PolyMeshNode* ship;
		renderer::Shader* shadowShader;
		
		int mouseOldX, mouseOldY;
		input::Gamepad* gamepad;
		bool useGamepad;
		bool useMouse;

		renderer::FrameBuffer* fbo;
		renderer::ShadowMap* shadowMap;

		bool showGizmos;

		time::Clock* clock;

		void init_vbo_vao(const renderer::Shader& shader, GLuint* vbo_id, GLuint* vao_id);
		void release_vbo_vao(GLuint* vbo_id, GLuint* vao_id);

		/**
		 * Poll the keyboard and process the inputs. should only be used for simple
		 * stuff. More sophisticated input would need its own input thread.
		 */
		void pollKeyboard(void);

		void pollMouse(void);

		void drawOverlays(void);
	};
}
