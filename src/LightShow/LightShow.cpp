#include "LightShow.h"

namespace murxy {

void LightShow::initialize() {
	for(uint8_t column = Columns::Left; column != Columns::Count; ++column) {
		mLights[column].begin(getAddress(static_cast<Columns>(column)));
	}
}

XSAA1064 &LightShow::operator[](Columns column) {
	return mLights[column];
}
const XSAA1064 &LightShow::operator[](Columns column) const {
	return mLights[column];
}

void LightShow::update() {
	for (auto &light : mLights)
		light.update();
}

void LightShow::clear() {
	for (auto &light : mLights)
		light.clear();
}

void LightShow::execute(uint16_t command) {
	constexpr uint16_t TargetMask   = 0b1000000000000000;
	constexpr uint16_t ColumnMask   = 0b0100000000000000;
	constexpr uint16_t CommandMask  = 0b0011000000000000;
	constexpr uint16_t FromLedsMask = 0b0000111110000000;
	constexpr uint16_t ToLedsMask   = 0b0000000001111100;
	constexpr uint16_t ValueMask    = 0b0000000000000010;
	constexpr uint16_t LinkMask     = 0b0000000000000001;

	enum Command {
		Clear, Set, Range, All
	};

	static const auto check = [](uint16_t command, uint16_t mask) -> bool {
		return (command & mask) != mask;
	};

	if (!check(command, TargetMask))
		return;

	const Columns column = static_cast<Columns>((command & ColumnMask) >> 14);
	const Command cmd = static_cast<Command>((command & CommandMask) >> 12);
	const bool enabled = (command & ValueMask) >> 1;
	const bool hasLink = check(command, LinkMask);

	switch(cmd) {
		case Command::Clear: {
			mLights[column].clear();
		} break;
		case Command::Set: {
			const uint8_t position = ((command & FromLedsMask) >> 7);
			mLights[column].set(position, enabled);
		} break;
		case Command::Range: {
			const uint8_t from = ((command & FromLedsMask) >> 7);
			const uint8_t to = ((command & ToLedsMask) >> 2);
			mLights[column].set(from, to, enabled);
		} break;
		case Command::All: {
			mLights[column].setAll(enabled);
		}
	}
	mLights[column].set(XSAA1064::LinkLed, hasLink);
}

XSAA1064 *LightShow::begin() {
	return mLights;
}
const XSAA1064 *LightShow::begin() const {
	return mLights;
}
XSAA1064 *LightShow::end() {
	return mLights + Columns::Count;
}
const XSAA1064 *LightShow::end() const {
	return mLights + Columns::Count;
}

uint8_t LightShow::getAddress(Columns c) {
    return 0b00111000 | (c << 0) | (c << 1);
}

} // namespace murxy
