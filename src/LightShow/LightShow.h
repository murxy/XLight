#if !defined(__LIGHT_SHOW__)
#define __LIGHT_SHOW__

#include <XSAA1064.h>

namespace murxy {

class LightShow {
public:
	enum Columns{ Left, Right, Count };

	void initialize();

	XSAA1064 &operator[](Columns column);
	const XSAA1064 &operator[](Columns column) const;

	/// @brief Update all light
	void update();

	/// @brief Clear all light including link light
	void clear();

	/**
	 * @brief Execute command
	 *
	 * @param command The data in format [T][C][CMD]
	 * Where is:
	 *     T - Target. Have to be 0. Otherwise the command will be ignored
	 *     C - Column. Left or right column
	 *   CMD - Command. 0 - clear, 1 - set, 2 - set (range), 3 - all
	 * @see in documentation @link
	 */
	void execute(uint16_t command);

	XSAA1064 *begin();
	const XSAA1064 *begin() const;
	XSAA1064 *end();
	const XSAA1064 *end() const;

	static uint8_t getAddress(Columns column);

private:
	XSAA1064 mLights[Columns::Count];
};

} // namespace murxy


#endif
