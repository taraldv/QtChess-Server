# QtChess-Server
Server for https://github.com/taraldv/QtChess <br>
It will listen on the specified port. <br>
And handle 4 different requests
<ul>
  <li>Host game</li>
  <li>Join game</li>
  <li>Request game list</li>
  <li>Execute move</li>
</ul>
After a game has started, if the connection is broken, the server will destroy the game instance.
