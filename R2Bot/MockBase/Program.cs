using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;
using R2D2;
using BaseComm;


namespace MockBase {

    class BotToBaseImpl : BotToBaseDisp_ {
        public BotPosition CurrentPosition { get; private set; }

        public override void stopAndGo(bool stopped, Ice.Current current)
        {
            throw new NotImplementedException();
        }

        public override void updateBotPosition(BotPosition enc, BotPosition imu, Ice.Current current)
        {
            CurrentPosition = enc;
            Console.WriteLine("BotPosition: x = {0}, y = {1}, theta = {2}",
                imu.x, imu.y, imu.theta);
        }


        public override void updateObstacleGrid(long[] grid, Ice.Current current)
        {
            //Console.WriteLine("\n\nObstacleGrid:");
            //bool[,] decodedGrid = BotConnection.DecodeObstacleGrid(grid);
            //for (int i = 0; i < BotConnection.GRID_HEIGHT; i++) {
            //    for (int j = 0; j < BotConnection.GRID_WIDTH; j++) {
            //        Console.Write(decodedGrid[i, j] ? 'X' : '.');
            //    }
            //    Console.WriteLine();
            //}
        }

        public override void updatePowerStats(float batteryPercent, float voltage,
                float regulatedCurrent, float unregulatedCurrent, Ice.Current current)
        {
            Console.WriteLine("PowerStats: % = {0}, V = {1}, regCur = {2}, unregCur = {3}",
                batteryPercent, voltage, regulatedCurrent, unregulatedCurrent);
        }

        public override void updateInventory(int inventory, Ice.Current current)
        {
            Console.WriteLine("Inventory: " + Convert.ToString(inventory, 2));
        }
    }

    class Program {

        static void Main(string[] args) {
            BotConnection botConn = null;
            while (true) {
                try {
                    var botToBase = new BotToBaseImpl();
                    botConn = new BotConnection(botToBase, "r2d2");
                    BaseToBotPrx bot = botConn.BotProxy;

                    while (true) {
                        Console.WriteLine("Choose a command:");
                        Console.WriteLine("  1. Play sound");
                        Console.WriteLine("  2. Move using XBox controller");
                        switch (Console.ReadKey(true).Key) {
                            case ConsoleKey.D1:
                                PlaySound(bot);
                                break;
                            case ConsoleKey.D2:
                                XboxControls(bot, botToBase);
                                break;
                        }
                    }

                } catch (Exception e) {
                    Console.Error.WriteLine(e.ToString());
                } finally {
                    if (botConn != null) { botConn.Dispose(); }
                }
            }
        }

        static void PlaySound(BaseToBotPrx bot) {
            Console.WriteLine("Which sound?");
            var sounds = Enum.GetValues(typeof(Sound)).Cast<Sound>();
            Console.WriteLine(string.Join(", ", sounds.Select((s, i) => i + ". " + s)));
            string line = Console.ReadLine();
            Sound snd = (Sound)Enum.Parse(typeof(Sound), line, true);
            bot.playSound(snd);
        }

        static void XboxControls(BaseToBotPrx bot, BotToBaseImpl botToBase) {
            // Adjustable constants
            const int commandFrequency = 20; // commands per second
            const float speedMul = 60;
            const float headMul = 2.0f;
            const int moveSpeed = 8;

            // Mutable state
            float headAngle = 0;
            GamePadButtons buttons = GamePad.GetState(PlayerIndex.One).Buttons;

            int prevLSpeed = 0;
            int prevRSpeed = 0;

            Console.WriteLine("Use the left and right thumbsticks to move the wheels");
            Console.WriteLine("Use the left and right triggers to rotate the head");

            while (true) {
                GamePadState state = GamePad.GetState(PlayerIndex.One);
                if (!state.IsConnected) {
                    Console.WriteLine("Gamepad not connected!");
                    bot.move(0, 0);
                    return;
                }

                // Calculate wheel speed
                int lSpeed, rSpeed;
                if (state.DPad.Up == ButtonState.Pressed) {
                    lSpeed = rSpeed = moveSpeed;
                } else if (state.DPad.Down == ButtonState.Pressed) {
                    lSpeed = rSpeed = -moveSpeed;
                } else if (state.DPad.Left == ButtonState.Pressed) {
                    bot.move(-moveSpeed, moveSpeed);
                    lSpeed = -moveSpeed;
                    rSpeed = moveSpeed;
                } else if (state.DPad.Right == ButtonState.Pressed) {
                    lSpeed = moveSpeed;
                    rSpeed = -moveSpeed;
                } else {
                    lSpeed = Convert.ToInt32(speedMul * state.ThumbSticks.Left.Y);
                    rSpeed = Convert.ToInt32(speedMul * state.ThumbSticks.Right.Y);
                }

                // Move; make sure we don't send the same speed repeatedly
                if (!(lSpeed == prevLSpeed && rSpeed == prevRSpeed)) {
                    bot.move(lSpeed, rSpeed);
                }
                prevLSpeed = lSpeed;
                prevRSpeed = rSpeed;

                // Calculate head angle
                float headAngleDelta = state.Triggers.Left - state.Triggers.Right;
                if (headAngleDelta != 0) {
                    headAngle += headMul * headAngleDelta;
                    bot.rotateHead((int) headAngle);
                }
                
                // Play sound?
                if (state.Buttons.A == ButtonState.Pressed && state.Buttons.A != buttons.A) {
                    bot.playSound(Sound.CHEERFUL);
                } else if (state.Buttons.B == ButtonState.Pressed && state.Buttons.B != buttons.B) {
                    bot.playSound(Sound.EXCITED);
                } else if (state.Buttons.X == ButtonState.Pressed && state.Buttons.X != buttons.X) {
                    bot.playSound(Sound.LAUGHING);
                } else if (state.Buttons.Y == ButtonState.Pressed && state.Buttons.Y != buttons.Y) {
                    bot.playSound(Sound.SAD);
                }

                // Move flap?
                if (state.Buttons.RightShoulder == ButtonState.Pressed
                        && state.Buttons.RightShoulder != buttons.RightShoulder) {
                    bot.setFlapState(FlapState.FLAPOPEN);
                } else if (state.Buttons.LeftShoulder == ButtonState.Pressed
                        && state.Buttons.LeftShoulder != buttons.LeftShoulder) {
                    bot.setFlapState(FlapState.FLAPCLOSED);
                }

                buttons = state.Buttons;

                Thread.Sleep(1000 / commandFrequency);
            }
        }
    }
}
