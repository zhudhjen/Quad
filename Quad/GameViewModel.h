#pragma once
#include <vector>
#include <map>

#include "common.h"

#include"PlayerMoveCommand.h"
#include"AiMoveCommand.h"
#include"GameStartCommand.h"
#include"UndoCommand.h"

#include "GameModel.h"

class GameViewModel :public Proxy_Notification<GameViewModel>,
	public GameStartCommand<GameViewModel>,
	public PlayerMoveCommand<GameViewModel>,
	public AiMoveCommand<GameViewModel>,
	public UndoCommand<GameViewModel>
					 
{
public:
	GameViewModel() throw() :GameStartCommand<GameViewModel>(this),
		PlayerMoveCommand<GameViewModel>(this),
		AiMoveCommand<GameViewModel>(this),
		UndoCommand<GameViewModel>(this) {}

	// Current player
	void setCurrentPlayer(const COLOR player) {
		m_current_player = player;
	}

	const COLOR getCurrentPlayer()const throw() {
		return m_current_player;
	}

	RefPtr<COLOR> getCurrentPlayerRef() throw() {
		return RefPtr<COLOR>(&m_current_player);
	}

	// If the game is over
	void setGameState(const GAME_STATE state) {
		m_game_state = state;
	}

	const GAME_STATE getGameState()const throw() {
		return m_game_state;
	}

	RefPtr<GAME_STATE> getGameStateRef() throw() {
		return RefPtr<GAME_STATE>(&m_game_state);
	}

	// Game mode
	void setGameMode(const GameMode mode) {
		m_game_mode = mode;
	}

	const GameMode getGameMode()const throw() {
		return m_game_mode;
	}

	RefPtr<GameMode> getGameModeRef() throw() {
		return RefPtr<GameMode>(&m_game_mode);
	}

	// Last move
	void setLastMove(const Position move) {
		m_last_move = move;
	}

	const Position getLastMove()const throw() {
		return m_last_move;
	}

	RefPtr<Position> getLastMoveRef() throw() {
		return RefPtr<Position>(&m_last_move);
	}

	// Status
	void setStatus(const std::map<Position, std::vector<COLOR>> status) {
		m_status = status;
	}

	const std::map<Position, std::vector<COLOR>> getStatus()const throw() {
		return m_status;
	}

	RefPtr<std::map<Position, std::vector<COLOR>>> getStatusRef() throw() {
		return RefPtr<std::map<Position, std::vector<COLOR>>>(&m_status);
	}

	// If each column is valid
	void setIsColumnValid(const std::map<Position, bool> column) {
		m_is_column_valid = column;
	}

	const std::map<Position, bool> getIsColumnValid()const throw() {
		return m_is_column_valid;
	}

	RefPtr<std::map<Position, bool>> getIsColumnValidRef() throw() {
		return RefPtr<std::map<Position, bool>>(&m_is_column_valid);
	}

	// Set Model
	void setGameModelRef(RefPtr<GameModel> model) {
		m_game_model = model;
	}

	// Game start command
	void gameStart(const GameMode mode) {
		m_game_model.Deref().gameStart(mode);
		setGameMode(mode);
		setGameState(m_game_model.Deref().getGameState());
		setStatus(m_game_model.Deref().getStatus());
		setIsColumnValid(m_game_model.Deref().getIsColumnValid());
		setCurrentPlayer(m_game_model.Deref().getCurrentPlayer());
		Fire_OnPropertyChange(std::string("GameMode"));
		Fire_OnPropertyChange(std::string("Status"));
		Fire_OnPropertyChange(std::string("ColumnValid"));
		Fire_OnPropertyChange(std::string("CurrentPlayer"));
		Fire_OnPropertyChange(std::string("GameState"));
		Fire_OnCommandComplete(std::string("GameStart"), true);
	}

	// Player move command
	void playerMove(const Position pos) {
		m_game_model.Deref().playerMove(pos);
		setLastMove(pos);
		setGameState(m_game_model.Deref().getGameState());
		setStatus(m_game_model.Deref().getStatus());
		setIsColumnValid(m_game_model.Deref().getIsColumnValid());
		setCurrentPlayer(m_game_model.Deref().getCurrentPlayer());
		Fire_OnPropertyChange(std::string("Status"));
		Fire_OnPropertyChange(std::string("ColumnValid"));
		Fire_OnPropertyChange(std::string("LastMove"));
		Fire_OnPropertyChange(std::string("GameState"));		
		Fire_OnPropertyChange(std::string("CurrentPlayer"));
		Fire_OnCommandComplete(std::string("PlayerMove"), true);
	}

	// AI move command
	void aiMove() {
		m_game_model.Deref().aiMove();
		setLastMove(m_game_model.Deref().getAiDecision());
		setGameState(m_game_model.Deref().getGameState());
		setStatus(m_game_model.Deref().getStatus());
		setIsColumnValid(m_game_model.Deref().getIsColumnValid());
		setCurrentPlayer(m_game_model.Deref().getCurrentPlayer());
		Fire_OnPropertyChange(std::string("Status"));
		Fire_OnPropertyChange(std::string("ColumnValid"));
		Fire_OnPropertyChange(std::string("LastMove"));
		Fire_OnPropertyChange(std::string("GameState"));
		Fire_OnPropertyChange(std::string("CurrentPlayer"));
		Fire_OnCommandComplete(std::string("AiMove"), true);
	}

	// Undo command
	void undo() {
		m_game_model.Deref().undo();
		setStatus(m_game_model.Deref().getStatus());
		setIsColumnValid(m_game_model.Deref().getIsColumnValid());
		setCurrentPlayer(m_game_model.Deref().getCurrentPlayer());
		Fire_OnPropertyChange(std::string("Status"));
		Fire_OnPropertyChange(std::string("ColumnValid"));
		Fire_OnPropertyChange(std::string("CurrentPlayer"));
		Fire_OnCommandComplete(std::string("Undo"), false);
	}

private:
	COLOR m_current_player;
	GAME_STATE m_game_state;
	Position m_last_move;
	GameMode m_game_mode;
	std::map<Position, std::vector<COLOR>> m_status;
	std::map<Position, bool> m_is_column_valid;

	RefPtr<GameModel> m_game_model;

};

