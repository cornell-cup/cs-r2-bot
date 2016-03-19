using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading;
using R2D2;

namespace BaseComm {

    public class BotConnection : IDisposable {
        public const int GRID_WIDTH = 60;
        public const int GRID_HEIGHT = 60;

        private const int PING_FREQUENCY = 100;
        private const int BOT_TO_BASE_PORT = 10000;
        private const int BASE_TO_BOT_PORT = 10001;

        private Ice.Communicator communicator;

        /// <summary>
        /// A proxy which may be used for communicating with R2D2.
        /// </summary>
        public BaseToBotPrx BotProxy { get; private set; }

        /// <summary>
        /// Represents a connection to R2D2. On construction, this object creates a two-way
        /// connection with the bot. To properly shutdown the connection, make sure that
        /// the Dispose method is called; this is especially important if you wish to create a
        /// new BotConnection later.
        /// </summary>
        /// <param name="botToBase">Object implementing functions that receive from the bot</param>
        /// <param name="hostname">Hostname of the bo.</param>
        public BotConnection(BotToBaseDisp_ botToBase, string hostname = "R2D2") {
            try {
                Console.WriteLine("Starting server");
                communicator = Ice.Util.initialize();

                Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithEndpoints(
                    "BotToBaseAdapter", "tcp -p " + BOT_TO_BASE_PORT);
                adapter.add(botToBase, communicator.stringToIdentity("BotToBaseImpl"));
                adapter.activate();

                Console.WriteLine("Connecting to R2D2");
                Ice.ObjectPrx prx = communicator.stringToProxy(
                    string.Format("BaseToBotImpl:tcp -h {0} -p {1}", hostname, BASE_TO_BOT_PORT));
                BotProxy = BaseToBotPrxHelper.checkedCast(prx);
                if (BotProxy == null)
                    throw new ApplicationException("Invalid proxy");

                // Pings are necessary to ensure that the connection is live.
                // We want R2D2 to stop immediately if the connection goes dead.
                new Thread(pinger).Start();

            } catch {
                if (communicator != null) { communicator.destroy(); }
                throw;
            }
        }

        /// <summary>
        /// Cleanup resources associated with the connection.
        /// </summary>
        public void Dispose() {
            communicator.destroy();
        }

        /// <summary>
        /// Decode the grid given by the `updateObstacleGrid` function.
        /// </summary>
        /// <param name="grid">Encoded grid</param>
        /// <returns>Decoded grid, with (0,0) corresponding to upper-left corner</returns>
        public static bool[,] DecodeObstacleGrid(long[] grid) {
            bool[,] result = new bool[GRID_HEIGHT, GRID_WIDTH];
            for (int i = 0; i < GRID_HEIGHT; i++) {
                long row = grid[i];
                long colMask = 1;
                for (int j = 0; j < GRID_WIDTH; j++) {
                    result[i, j] = (row & colMask) != 0;
                    colMask = colMask << 1;
                }
            }
            return result;
        }

        private void pinger() {
            try {
                while (true) {
                    BotProxy.ping();
                    Thread.Sleep(PING_FREQUENCY);
                }
            } catch {
                // We don't care if this thread dies
            }
        }


    }
}
