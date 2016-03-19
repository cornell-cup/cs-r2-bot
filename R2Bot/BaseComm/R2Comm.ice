module R2D2 {

	// The current position and orientation of the bot.
	struct BotPosition {
		float x;      // meters
		float y;      // meters
		float theta;  // radians (ccw from x-axis)
	};

	// An obstacle detected by the bot, represented as a grid square.
	// The `blockage` is in the range [0,9] and represents the likelihood the
	// square is occupied, with 0 being empty and 9 being occupied.
	struct Obstacle {
		int x;
		int y;
		int blockage;
	};

	sequence<Obstacle> ObstacleList;

	sequence<long> ObstacleGrid;

	// R2D2 sounds
	enum Sound {
		ACKNOWLEDGE,
		ANXIOUS,
		CHEERFUL,
		EXCITED,
		LAUGHING,
		SCREAM,
		SAD
	};

	// The state of R2D2's head flap
	enum FlapState {
		FLAPOPEN,
		FLAPCLOSED
	};

	// Functions where Bot is the client and Base is the server.
	interface BotToBase {
		void updateBotPosition(BotPosition enc, BotPosition imu);
		void updateObstacleGrid(ObstacleGrid grid);
		void updatePowerStats(float batteryPercent, float voltage,
				float regulatedCurrent, float unregulatedCurrent);
		void updateInventory(int inventory);
		void stopAndGo(bool stopped);
	};

	// Functions where Base is the client and Bot is the server.
	interface BaseToBot {
		void ping();
		void playSound(Sound snd);
		void move(int lSpeed, int rSpeed);
		void rotateHead(int angle);
		void setFlapState(FlapState state);
		void shutdown();
	};

};