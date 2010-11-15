/*
 * A basic camera modelled after a real 35mm camera.
 */

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

class Camera
{
public:
	Camera();
	virtual ~Camera();

private:
	/**
	 * Set the film gate in pixel
	 * Set the film gate to specific size in pixels. This will mostly be the
	 * final output resolution, or the visual area in a letterboxed output.
	 * @param width The width of the gate
	 * @param height The height of the gate
	 */
	void setGateInPixel(int width, int height);

	/**
	 * The filter margin of the camera.
	 * A field wider than the final gate is rendered to provide some
	 * margin for post processing filters.
	 */
	void setFilterMargininPixel(int horizontalMargin, int verticalMargin);

	/**
	 * Set the 35mm equivalent focal length of the lens.
	 * @param The focal length as it would be in a standard 35mm film camera.
	 */
	void setFocalLength(double mm);

	/**
	 * The counterpart to setFocalLength. Directly set the horizontal AOV
	 * @param radians The horizontal AOV in radians.
	 */
	void setAngleOfView(double radians);

	//TODO: position, orientation, movement
	//TODO: focus, vertigo

	//TODO: color correction post thingys
};

#endif /* CAMERA_HPP_ */
