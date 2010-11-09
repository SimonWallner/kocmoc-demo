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
	void draw();

	/**
	 * init everything that is needed for the game
	 */
	void init();


private:

	Kocmoc();
	Kocmoc(const Kocmoc &cc); // protect the copy constructor as well
	~Kocmoc();

	static Kocmoc *instance;

	bool running;

};
