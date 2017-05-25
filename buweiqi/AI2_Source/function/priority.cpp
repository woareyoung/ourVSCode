#include "../../stdafx.h"
#include "../../AI2_Header/AI2.h"

int AI2::priority_score(int scoreBase, int scorePRI, int type) {
	return turn2Who == type ? scorePRI : scoreBase;
}
