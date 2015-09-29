#ifndef __AI_H__
#define __AI_H__

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "PlaygroundStatus.h"

const long LINE_WORTH[5] = { 1, 10, 100, 10000, 100000000 };
const float FUTURE_WORTH = 0.125;

const long MAX_DEPTH[3] = { 1, 2, 4 };
const long MAX_UNDO_COUNT[3] = { 3, 1, 0 };
const long RAND_RANGE[3] = { 500, 100, 10 };

enum PLAYER_TYPE {
	REAL = 0, COMP
};

// STL is slow without O2, using native structure to enable medium calculation
struct SNativeState {

	bool m_status[4][4][4];
	int m_count[4][4];
	int m_move_count;
	bool m_current_player;
	GAME_STATE m_game_state;

	SNativeState() : m_current_player(COLOR::BLACK), m_move_count(0), m_game_state(GAME_STATE::PLAYING) {
		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < 4; j++) {
				m_count[i][j] = 0;
			}
		}
	}
	
	SNativeState(PlaygroundStatus& status) {
		m_current_player = status.getCurrentPlayer();
		m_game_state = status.getGameState();
		m_move_count = status.getMoveCount();
		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < 4; j++) {
				Position pos(i, j);
				m_count[i][j] = status.getStatus().at(pos).size();
				for (size_t k = 0; k < m_count[i][j]; k++) {
					m_status[i][j][k] = status.getStatus().at(pos).at(k);
				}
			}
		}
	}
	
	SNativeState(SNativeState& param) : m_current_player(param.m_current_player) {
		memcpy(m_status, param.m_status, 64 * sizeof(bool));
		memcpy(m_count, param.m_count, 16 * sizeof(int));
	}
	
	void move(int x, int y) {
		m_status[x][y][m_count[x][y]++] = m_current_player;
		for (int i = 0; i < 13; ++i) {
			if (countBeads(m_current_player, x, y, m_count[x][y] - 1, DX[i], DY[i], DZ[i]) == 4) {
				m_game_state = GAME_STATE::WIN;
				return;
			}
		}

		++m_move_count;
		if (64 == m_move_count) {
			m_game_state = GAME_STATE::DRAW;
			return;
		}
		m_current_player = m_current_player == COLOR::BLACK ? COLOR::WHITE : COLOR::BLACK;
	}
	
	const long countBeads(const bool color, const long x, const long y, const long z, const long dx, const long dy, const long dz) {
		long cnt = 0, tot = 0;
		long tx = 0, ty = 0, tz = 0;

		for (int i = 0; i < 4; ++i) {
			tx = x + dx * i;
			ty = y + dy * i;
			tz = z + dz * i;
			if (!inRange(tx) || !inRange(ty) || !inRange(tz)) {
				break;
			}
			++cnt;
			if (m_count[tx][ty] > tz) {
				if (color == m_status[tx][ty][tz]) {
					++tot;
				} else {
					return -1;
				}
			}
		}
		for (int i = 1; i < 4; ++i) {
			tx = x - dx * i;
			ty = y - dy * i;
			tz = z - dz * i;
			if (!inRange(tx) || !inRange(ty) || !inRange(tz)) {
				break;
			}
			++cnt;
			if (m_count[tx][ty] > tz) {
				if (color == m_status[tx][ty][tz]) {
					++tot;
				} else {
					return -1;
				}
			}
		}
		return cnt == 4 ? tot : 0;
	}
};

class AI {
public:
	explicit AI(void);
	~AI(void);

	void gameStart(const LEVEL level);

	void search(PlaygroundStatus &playground_status, const LEVEL level);

	const Position getCompMove(void) const;
private:
	LEVEL m_level;

	long m_max_depth;

	const COLOR m_real_color;
	const COLOR m_comp_color;

	float total_worth[2];
	float point_worth[4][4][4][2];

	Position m_comp_move;

	void clearWorth(void);

	void calcLineWorth(SNativeState &native_status, const long x, const long y, const long z, const long dx, const long dy, const long dz);

	void calcWorth(SNativeState &native_status);

	const float getValuation(SNativeState &native_status, const PLAYER_TYPE player_type) const;

	const float simpleRecursive(SNativeState &native_status, const long depth);

	const float complexRecursive(SNativeState &native_status, const long depth, const long k);
};

AI::AI(void) :
	m_level(LEVEL::EASY),
	m_max_depth(),
	m_real_color(COLOR::BLACK),
	m_comp_color(COLOR::WHITE),
	m_comp_move(-1, -1)
{
	m_max_depth = 0;

	clearWorth();

	m_comp_move = Position(-1, -1);
}
AI::~AI(void) {

}

void AI::gameStart(const LEVEL level) {
	m_level = level;

	m_max_depth = MAX_DEPTH[level];

	clearWorth();

	m_comp_move = Position(-1, -1);

	std::srand(time(0));
}

void AI::clearWorth(void) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				point_worth[i][j][k][PLAYER_TYPE::REAL] = point_worth[i][j][k][PLAYER_TYPE::COMP] = 0;
			}
		}
	}

	total_worth[PLAYER_TYPE::REAL] = total_worth[PLAYER_TYPE::COMP] = 0;
}

void AI::calcLineWorth(SNativeState &native_status, const long x, const long y, const long z, const long dx, const long dy, const long dz) {
	long i = 0, j = 0, k = 0;
	long bead_count[2] = { 0, 0 };

	bead_count[PLAYER_TYPE::REAL] = native_status.countBeads(m_real_color, x, y, z, dx, dy, dz);
	bead_count[PLAYER_TYPE::COMP] = native_status.countBeads(m_comp_color, x, y, z, dx, dy, dz);

	for (int t = 0; t < 4; ++t) {
		i = x + t * dx;
		j = y + t * dy;
		k = z + t * dz;

		if (k >= native_status.m_count[i][j]) {
			if (bead_count[PLAYER_TYPE::COMP] < 1 && bead_count[PLAYER_TYPE::REAL] != -1) {
				point_worth[i][j][k][PLAYER_TYPE::REAL] += LINE_WORTH[bead_count[PLAYER_TYPE::REAL]];
			}
			if (bead_count[PLAYER_TYPE::REAL] < 1 && bead_count[PLAYER_TYPE::COMP] != -1) {
				point_worth[i][j][k][PLAYER_TYPE::COMP] += LINE_WORTH[bead_count[PLAYER_TYPE::COMP]];
			}
		}
	}
}

void AI::calcWorth(SNativeState &native_status) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			calcLineWorth(native_status, i, j, 0, 0, 0, 1);
			calcLineWorth(native_status, i, 0, j, 0, 1, 0);
			calcLineWorth(native_status, 0, i, j, 1, 0, 0);
		}
		calcLineWorth(native_status, i, 0, 0, 0, 1, 1);
		calcLineWorth(native_status, 0, i, 0, 1, 0, 1);
		calcLineWorth(native_status, 0, 0, i, 1, 1, 0);
		calcLineWorth(native_status, i, 0, 3, 0, 1, -1);
		calcLineWorth(native_status, 3, i, 0, -1, 0, 1);
		calcLineWorth(native_status, 0, 3, i, 1, -1, 0);
	}

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			for (int k = 0; k < 2; ++k) {
				calcLineWorth(native_status, 3 * i, 3 * j, 3 * k, 1 - (i << 1), 1 - (j << 1), 1 - (k << 1));
			}
		}
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = native_status.m_count[i][j] - 1; k < 4; ++k) {
				point_worth[i][j][k][PLAYER_TYPE::REAL] *= std::powf(FUTURE_WORTH, k - native_status.m_count[i][j]);
				point_worth[i][j][k][PLAYER_TYPE::COMP] *= std::powf(FUTURE_WORTH, k - native_status.m_count[i][j]);
			}
			for (int k = 2; k >= native_status.m_count[i][j]; --k) {
				point_worth[i][j][k][PLAYER_TYPE::REAL] += point_worth[i][j][k + 1][PLAYER_TYPE::REAL];
				point_worth[i][j][k][PLAYER_TYPE::COMP] += point_worth[i][j][k + 1][PLAYER_TYPE::COMP];
			}
		}
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				total_worth[PLAYER_TYPE::REAL] += point_worth[i][j][k][PLAYER_TYPE::REAL];
				total_worth[PLAYER_TYPE::COMP] += point_worth[i][j][k][PLAYER_TYPE::COMP];
			}
		}
	}
}
const float AI::getValuation(SNativeState &native_status, const PLAYER_TYPE player_type) const {
	int rand_num = rand() % 10000 * RAND_RANGE[m_level] / 10000;
	return player_type == PLAYER_TYPE::REAL ? 
		 0.6 * total_worth[PLAYER_TYPE::REAL] - 0.4F * total_worth[PLAYER_TYPE::COMP] + rand_num :
		-0.6 * total_worth[PLAYER_TYPE::REAL] + 0.4F * total_worth[PLAYER_TYPE::COMP] + rand_num;
}

void AI::search(PlaygroundStatus &playground_status, const LEVEL level) {
	float max_value = -1.0E+20F, value = 0;
	Position result(-1, -1), pos(-1, -1);
	std::map<Position, bool> isColumnValid = playground_status.getIsColumnValid();
	SNativeState param(playground_status);

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {

			if (param.m_count[i][j] < 4) {
				SNativeState status = param;
				status.move(i, j);

				if (status.m_game_state == GAME_STATE::WIN) {
					m_comp_move = Position(i, j);
					return;
				}
				
				value = -simpleRecursive(status, MAX_DEPTH[level] - 1);

				if (max_value < value) {
					max_value = value;
					result = Position(i, j);
				}
			}
		}
	}

	m_comp_move = result;
}

const float AI::simpleRecursive(SNativeState &native_status, const long depth) {
	if (depth == 0) {
		clearWorth();
		calcWorth(native_status);
		return getValuation(native_status, m_level == LEVEL::EASY ? PLAYER_TYPE::REAL : PLAYER_TYPE::COMP);
	} else {
		float ret = -1.0E+20F, value = 0;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (native_status.m_count[i][j] < 4) {
					SNativeState status = native_status;
					status.move(i, j);

					if (status.m_game_state == GAME_STATE::WIN) {
						return 1.0E+10F;
					} else {
						value = -simpleRecursive(status, depth - 1);
						if (ret < value) {
							ret = value;
						}
					}
				}
			}
		}
		return ret;
	}
}

const float AI::complexRecursive(SNativeState &native_status, const long depth, const long k) {
	if (depth == 0) {
		return simpleRecursive(native_status, depth);
	} else {
		float ret = 0, value = 0;
		std::priority_queue<std::pair<float, int>> heap;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (native_status.m_count[i][j] < 4) {
					SNativeState status = native_status;
					status.move(i, j);

					if (status.m_game_state == GAME_STATE::WIN) {
						return 1.0E+10F;
					} else {
						value = -simpleRecursive(status, depth);
					}

					heap.push(std::pair<float, int>(value, (i<<2)+j));
				}
			}
		}

		ret = -1.0E+30F;

		for (int i = 0; i < 4; ++i) {
			SNativeState status = native_status;

			int pos = heap.top().second;
			heap.pop();

			status.move(pos >> 2, pos & 3);

			value = -complexRecursive(status, depth - (k & 1), k - 1);

			if (ret < value) {
				ret = value;
			}
		}
		return ret;
	}
}

const Position AI::getCompMove(void) const {
	return m_comp_move;
}

#endif
