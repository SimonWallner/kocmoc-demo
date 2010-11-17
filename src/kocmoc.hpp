#include "shader.hpp"
#include "Timer.hpp"
#include "Camera.hpp"

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
	void draw(const Shader &shader, GLuint vao_id);

	/**
	 * init everything that is needed for the game
	 */
	void init();

	bool isRunning();


private:

	Kocmoc();
	Kocmoc(const Kocmoc &cc); // protect the copy constructor as well
	~Kocmoc();

	static Kocmoc *instance;

	bool running;
	Shader *minimal;
	GLuint vao_id;

	Timer timer;

	Camera* camera;

	void init_vbo_vao(const Shader &shader, GLuint *vbo_id, GLuint *vao_id);
	void release_vbo_vao(GLuint *vbo_id, GLuint *vao_id);

	/**
	 * Poll the keyboard and process the inputs. should only be used for simple
	 * stuff. More sophisticated input would need its own input thread.
	 */
	void pollKeyboard(void);


};
