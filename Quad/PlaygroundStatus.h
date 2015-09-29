#ifndef __PLAYGROUND_STATUS_H__
#define __PLAYGROUND_STATUS_H__

#include <assert.h>
#include <map>
#include <vector>
#include <queue>
#include "common.h"

const long DX[13] = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,-1 };
const long DY[13] = { 0, 1, 1, 1, 0, 0, 0, 1,-1, 1, 1,-1, 1 };
const long DZ[13] = { 1, 0, 1,-1, 0, 1,-1, 0, 0, 1,-1, 1, 1 };

const bool inline inRange(const long t) {
	return t > -1 && t < 4;
}

class PlaygroundStatus {
public:
	explicit PlaygroundStatus(void) :
		m_current_player(COLOR::BLACK),
		m_status(),
		m_is_column_valid(),
		m_history(),
		m_move_count(0),
		m_game_state(GAME_STATE::DRAW)
	{

	}
	void gameStart(void) {
		m_current_player = COLOR::BLACK;
		m_status.clear();

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m_is_column_valid[Position(i, j)] = true;
				m_status[Position(i, j)].clear();
			}
		}

		m_history.clear();

		m_move_count = 0;
		m_game_state = GAME_STATE::PLAYING;
	}

	void setMove(const Position &pos) {
		if (GAME_STATE::PLAYING != m_game_state) {
			return;
		}

		m_status[pos].push_back(m_current_player);

		m_is_column_valid[pos] = m_status[pos].size() < 4;

		m_history.push_back(pos);

		for (int i = 0; i < 13; ++i) {
			if (countBeads(m_current_player, pos.getX(), pos.getY(), m_status[pos].size() - 1, DX[i], DY[i], DZ[i]) == 4) {
				m_game_state = GAME_STATE::WIN;
				return;
			}
		}

		++m_move_count;
		if (64 == m_move_count) {
			m_game_state = GAME_STATE::DRAW;
			return;
		}

		m_current_player = ((m_current_player == COLOR::BLACK) ? COLOR::WHITE : COLOR::BLACK);
	}
	void undo(void) {
		Position pos = m_history.back();

		m_history.pop_back();

		m_status[pos].pop_back();

		m_is_column_valid[pos] = true;
	}

	const long countBeads(const COLOR color, const long x, const long y, const long z, const long dx, const long dy, const long dz) {
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
			if (m_status[Position(tx, ty)].size() > tz) {
				if (color == m_status[Position(tx, ty)][tz]) {
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
			if (m_status[Position(tx, ty)].size() > tz) {
				if (color == m_status[Position(tx, ty)][tz]) {
					++tot;
				} else {
					return -1;
				}
			}
		}

		return cnt == 4 ? tot : -1;
	}
	const COLOR getCurrentPlayer(void) const {
		return m_current_player;
	}
	const std::map<Position, std::vector<COLOR>> getStatus(void) const {
		return m_status;
	}
	const std::map<Position, bool> getIsColumnValid(void) const {
		return m_is_column_valid;
	}
	const GAME_STATE getGameState(void) const {
		return m_game_state;
	}

	const int getMoveCount(void) const {
		return m_move_count;
	}

private:
	COLOR m_current_player;

	std::map<Position, std::vector<COLOR>> m_status;
	std::map<Position, bool> m_is_column_valid;

	std::vector<Position> m_history;

	long m_move_count;
	GAME_STATE m_game_state;
};

#endif
