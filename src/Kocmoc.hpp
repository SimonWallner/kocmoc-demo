#include "Shader.hpp"
#include "Timer.hpp"
#include "FilmCamera.hpp"
#include "KocmocScene.hpp"
#include "Gamepad.hpp"
#include "FrameBuffer.hpp"
#include "ShadowMap.hpp"
#include "OrthoCam.hpp"

namespace kocmoc
{

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

		FilmCamera *getCamera(void) {return camera;};
		Camera *getOrthoCam(void) {return orthoCam;};

		/**
		 * reload some stuff
		 */
		void reload(void);

	private:

		Kocmoc();
		Kocmoc(const Kocmoc &cc); // protect the copy constructor as well
		~Kocmoc();

		static Kocmoc *instance;

		bool running;
		
		Timer timer;

		FilmCamera* camera;
		OrthoCam *orthoCam;

		KocmocScene* scene;
		KocmocScene* ship;
		Shader *shadowShader;
		
		int mouseOldX, mouseOldY;
		input::Gamepad *gamepad;
		bool useGamepad;
		bool useMouse;

		FrameBuffer *fbo;
		ShadowMap *shadowMap;

		bool showGizmos;

		void init_vbo_vao(const Shader &shader, GLuint *vbo_id, GLuint *vao_id);
		void release_vbo_vao(GLuint *vbo_id, GLuint *vao_id);

		/**
		 * Poll the keyboard and process the inputs. should only be used for simple
		 * stuff. More sophisticated input would need its own input thread.
		 */
		void pollKeyboard(void);

		void pollMouse(void);

		void drawOverlays(void);

		bool foo;

	};
}
