#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "AI.h"

class GameModel {
public:
	explicit GameModel(void);
	~GameModel(void);

	void setAIRef(const RefPtr<AI> &ai);

	/* Game start */
	void gameStart(const GameMode &game_mode);

	/* Move */
	void playerMove(const Position &pos);

	/* AI Move */
	void aiMove(void);

	/* Undo */
	void undo(void);

	/* Get playground status */
	PlaygroundStatus getPlaygroundStatus(void) const;

	/* Get game mode */
	const GameMode getGameMode(void) const;

	/* Get current player */
	const COLOR getCurrentPlayer(void) const;

	/* Get status */
	const std::map<Position, std::vector<COLOR>> getStatus(void) const;

	/* Get is column valid */
	const std::map<Position, bool> getIsColumnValid(void) const;

	/* Get undo count */
	const long getUndoCount(void) const;

	/* Get is game over */
	const GAME_STATE getGameState(void) const;

	/* Get AI decision */
	const Position getAiDecision(void) const;
private:
	GameMode m_game_mode;

	PlaygroundStatus m_playground_status;

	AI m_ai;

	long m_undo_count;

	Position m_ai_decision;
};

GameModel::GameModel(void) :
	m_game_mode(false, LEVEL::EASY),
	m_ai(),
	m_playground_status(),
	m_undo_count(0),
	m_ai_decision(-1, -1)
{

}
GameModel::~GameModel(void) {

}

void GameModel::gameStart(const GameMode &game_mode) {
	m_game_mode = game_mode;

	m_ai.gameStart(game_mode.getLevel());

	m_playground_status.gameStart();

	m_undo_count = 0;

	m_ai_decision = Position(-1, -1);
}

void GameModel::playerMove(const Position &pos) {
	if (getGameState() != GAME_STATE::PLAYING) {
		return;
	}

	m_playground_status.setMove(pos);

	if (m_undo_count < MAX_UNDO_COUNT[m_game_mode.getLevel()]) {
		++m_undo_count;
	}
}
void GameModel::aiMove(void) {
	m_ai.search(m_playground_status, m_game_mode.getLevel());

	m_ai_decision = m_ai.getCompMove();

	playerMove(m_ai_decision);
}
void GameModel::undo(void) {
	m_playground_status.undo();
}

PlaygroundStatus GameModel::getPlaygroundStatus(void) const {
	return m_playground_status;
}
const GameMode GameModel::getGameMode(void) const {
	return m_game_mode;
}
const COLOR GameModel::getCurrentPlayer(void) const {
	return m_playground_status.getCurrentPlayer();
}
const std::map<Position, std::vector<COLOR>> GameModel::getStatus(void) const {
	return m_playground_status.getStatus();
}
const std::map<Position, bool> GameModel::getIsColumnValid(void) const {
	return m_playground_status.getIsColumnValid();
}
const long GameModel::getUndoCount(void) const {
	return m_undo_count;
}
const GAME_STATE GameModel::getGameState(void) const {
	return m_playground_status.getGameState();
}
const Position GameModel::getAiDecision(void) const {
	return m_ai_decision;
}

#endif
