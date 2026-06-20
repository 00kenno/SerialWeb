#pragma once

constexpr char html[] = R"(

<!DOCTYPE html>
<html lang='ja'>

<head>
  <meta charset='utf-8' />
  <meta name='viewport' content='width=device-width,initial-scale=1' />
  <title>TORICA SerialWeb</title>
</head>
<style>
  :root {
    font-family: sans-serif;
    padding: 20px;
    scroll-behavior: smooth;
  }
  h1 {
    font-size: xx-large;
    text-align: center;
  }
  svg {
    display: block;
    margin: 20px auto;
  }
  #tabBar {
    max-width: 800px;
    display: flex;
    justify-content: left;
    margin: 0 auto;
    gap: 5px;
    user-select: none; /* 選択を禁止する */
  }
  #tabBar div {
    padding: 10px 20px;
    font-size: large;
    font-weight: bold;
    cursor: pointer;
    border-top: 3px solid silver;
    border-left: 3px solid silver;
    border-right: 3px solid silver;
    border-radius: 10px 10px 0 0;
    background-color: white;
    position: relative;
    z-index: 50;
  }
  #tabBar div:active {
    background-color: #DDD;
  }
  #tabBar .active {
    z-index: 100;
  }
  .content {
    position: relative;
    box-sizing: border-box;
    max-width: 800px;
    margin: -3px auto 20px auto;
    padding: 15px;
    border: 3px solid silver;
    border-radius: 0 10px 10px 10px;
    z-index: 50;
  }
  #dashboardWrapper {
    display: flex;
    flex-direction: column;
    justify-content: center;
    gap: 7px;
  }
  #dashboardWrapper > div {
    display: flex;
    gap: 5px;
    border: gray solid 1px;
    border-radius: 5px;
    padding: 5px;
    min-width: 100px;
    flex-grow: 1;
  }
  #dashboardWrapper > div.update {
    animation-duration: 200ms;
    animation-name: update;
  }
  @keyframes update {
    from {
      box-shadow: 0px 0px 2px 2px gold;
    }
    to {
      box-shadow: none;
    }
  }
  #dashboardWrapper div .label {
    display: flex;
    flex-direction: row;
    justify-content: center;
    align-items: center;
    flex-basis: 50%;
    text-align: center;
    background-color: #EEE;
    border-radius: 5px;
    padding: 10px 5px;
    font-weight: bold;
  }
  #dashboardWrapper div .value {
    display: flex;
    flex-direction: row;
    justify-content: center;
    align-items: center;
    flex-basis: 50%;
    text-align: center;
    padding: 10px 5px;
  }
  #logWindow {
    display: flex;
    flex-direction: column;
    font-family: monospace;
    margin: 0;
    border: slategray 1px solid;
    min-height: 300px;
    overflow-x: auto;
    white-space: nowrap;
    background-color: #f9f9f9;
    padding: 3px;
  }
  #logWindow > div {
    padding: 3px;
    > span {
      padding: 2px;
      border-bottom: 1px dotted gray;
    }
  }
  #statusWrapper {
    background-color: #EEE;
    border: gray solid 1px;
    border-radius: 5px 5px 0 0;
    margin: 0 0 -1px 0;
    padding: 5px;
  }
  #logForm {
    margin-bottom: 7px;
    display: flex;
    gap: 5px;
    > .button {
      margin: 0;
      height: auto;
    }
  }
  #logInput {
    padding: 6px;
    font-family: monospace;
    border: slategray 1px solid;
    border-radius: 5px;
    flex: 1;
  }
  #footer {
    background-color: #EEE;
    margin-top: -1px;
    height: 1rem;
    border: slategray 1px solid;
    border-radius: 0 0 5px 5px;
  }
  .button {
    display: inline-flex;
    margin-bottom: 7px;
    padding: 4px 8px;
    font-weight: bold;
    font-size: small;
    cursor: pointer;
    color: buttontext;
    text-indent: 0px;
    border: slategray 1px solid;
    border-radius: 5px;
    box-sizing: border-box;
    box-shadow: 1px 1px 2px 0 black;
    background-color: buttonface;
    user-select: none; /* 選択を禁止する */
    align-items: center;
  }
  .button:active {
    background-color: #DDD;
  }
  .button.active {
    box-shadow: inset 2px 2px 2px 0 black;
  }
  #goToTopButton {
    position: fixed;
    bottom: 20px;
    right: 20px;
    width: 70px;
    height: 70px;
    color: white;
    background-color: gray;
    border-radius: 30px;
    cursor: pointer;
    user-select: none; /* 選択を禁止する */
    text-align: center;
    transition: opacity 0.5s;
    > span {
      position: absolute;
      width: 30px;
      height: 30px;
      rotate: 45deg;
      top: 25px;
      left: 20px;
      border-top: 7px solid white;
      border-left: 7px solid white;
      box-sizing: border-box;
    }
  }
</style>

<body>
  <svg width='350' height='150' xmlns='http://www.w3.org/2000/svg'>
    <g style='display:inline'>
      <path style='fill:#000;fill-opacity:1;stroke:#000;stroke-width:.475027;stroke-opacity:1'd='M434 252s48 12 107 35c12 5 40 18 31 29-8 10-30 14-48 16l9-9c3-3 6-6 8-9 2-4-1-5-4-3l-3 1c-1 1-3 2-5 3l-4-16-3-6h-10l-15 32h10l2-8h13c0 4-3 5-5 6-5 4-11 7-17 11-29 3-60 3-111 2-31 0-73-2-107-4 1-5 1-11 1-11s-9 4-13 11c-11-1-30-2-30-2s13 1 29 2c-3 7-11 31-9 46 1 8 6 17 16 16 11-1 4-13 8-19 2-2 10 0 13 0 1 3 7 2 10 2 10 1 20 0 29 2 7 1 13 3 20 3 4 0 10-2 13 0 5 2 9 6 14 8s10 3 15 5c2 2 3 6 6 6s7-2 10-2c9 1 18 1 27 1 4 0 8 1 12-1 3-1 0-4-2-5-4-4-8-9-14-10 0-3 3-3 6-4 6-1 12-3 17-5 18-6 33-18 47-30 5-3 9-7 14-11 25-3 52-8 59-19 13-19-19-30-51-39-66-20-95-24-95-24zm-200 11v12l28 2-3 31v10h22l2-38 27 2v-12zm110 11c-1 0-3 0-5 1-8 1-15 3-21 8-7 6-7 19-1 27 10 11 24 13 38 11 16-2 32-19 20-35-7-8-19-12-31-11zm44 6-2 42h14v-16c11 0 19 5 19 16l13 1c0-6-5-16-11-19v-1c2-1 6-1 8-3 5-6 0-14-6-16-10-5-24-3-35-4zm-42 4c6 0 12 1 16 6 6 9-2 21-12 23-6 0-14-2-17-6-6-9 0-22 10-23h3zm95 1-2 37 11 1 3-38zm36 3c-9 0-17 4-19 14-1 6 1 13 5 18 10 9 27 6 32-7l-10-1c-1 6-10 7-14 3-7-6-3-22 8-20 4 0 5 4 7 5 2 2 6 1 9 1 0-7-8-11-14-12h-4zm-75 1h7c2 0 4 1 5 1 4 1 6 4 3 7-2 2-13 2-15 0-1-2 0-6 0-8zm116 10 2 12h-8zm-236 34c30 2 69 4 100 5 50 0 94 0 115-1-5 3-10 7-14 9-10 7-20 13-30 19-6 3-13 9-20 11-4 1-9 3-12 3s-6-2-8-2c-6-1-14-2-20-2-4 0-8-1-12 0-2 0-2 2-5 2-5 0-10-2-15-3-15-4-28-7-41-15-6-4-13-7-18-13-2-2-3-6-6-8 1 14 12 22 22 29 5 2 11 4 14 8-5-2-18 2-21-2-3 0-5 0-7-2 4 0 10 0 14-2v-1h-24c-2 3-11 3-12-1-2-11 0-29 0-34z'transform='translate(-233.5 -251.506)' />
    </g>
  </svg>
  <div id='tabBar'>
    <div id='dashboardTab' onclick='changeTab(this.id)'>Dashboard</div>
    <div id='logTab' onclick='changeTab(this.id)'>Log</div>
  </div>
  <div id='dashboardWrapper' class='content'></div>
  <div id='logWrapper' class='content'>
    <form id='logForm'>
      <input type='text' id='logInput' placeholder='Type a message...' />
      <button class='button' id='logSendButton'>Send</button>
    </form>
    <div class='button' id='connectButton'>Connect</div>
    <div class='button' id='pauseButton'>Pause</div>
    <div class='button' id='clearButton'>Clear</div>
      <div class='button active' id='logTimestamps'>Show Timestamps</div>
    <div id='statusWrapper'>Status: <span id='status'></span></div>
    <div id='logWindow'></div>
    <div id='footer'></div>
    <div id='goToTopButton' style='display: none;'><span></span></div>
  </div>

  <script src='demo.js' type='text/javascript'></script>
  <script type='text/javascript'>

    let ws; // グローバル変数として websocket を保持
    const status = document.getElementById('status'); // ステータスを表示するdiv要素
    const connectButton = document.getElementById('connectButton'); // 


    function initWebSocket() { // WebSocket接続の初期化（または再接続）
      addLogLn('Attempting to connect WebSocket...');
      ws = new WebSocket(`ws://${location.hostname}/ws`);// ポート80はHTTPのデフォルトなので、:80 は省略可能
      status.textContent = 'Connecting...';
      status.style.color = 'black';

      ws.onopen = () => { // 接続成功時の処理
        addLogLn('WebSocket connected');
        status.textContent = 'Connected';
        status.style.color = 'green';
        connectButton.textContent = 'Disconnect';
      };

      ws.onclose = () => { // 切断時の処理
        addLogLn('WebSocket disconnected.');
        status.textContent = 'Disconnected';
        status.style.color = 'darkorange';
        connectButton.textContent = 'Connect';
      };

      ws.onmessage = (event) => { // メッセージ受信時の処理
        // addLog(`Received: ${event.data}`);
        // jsonデータのパース
        const data = JSON.parse(event.data);
        if (data.hasOwnProperty('index') && data.hasOwnProperty('label')) {
        updateDashbordItem(data.index, data.label, data.value);
        }
        else if (data.hasOwnProperty('value')) {
          addLog(data.value);
        }        
      };

      ws.onerror = (event) => {
        console.error('WebSocket Error: ', event);
        // エラー時も onclose が呼ばれることが多いので、再接続は onclose に任せる
        status.textContent = 'Error';
        status.style.color = 'red';
      };
    }


    // ページが読み込まれたら、最初の接続を開始する
    window.addEventListener('load', initWebSocket);

    connectButton.addEventListener('click', () => {
      if (status.textContent === 'Disconnected') {
        initWebSocket();
      }
      else {
        ws.close(1000, 'User requested disconnect');
      }
    });


    // タブの切り替え処理
    function changeTab(selectedTabId) {
      //console.log(`Changing to tab: ${selectedTab.id}`);
      const dashboardWrapper = document.getElementById('dashboardWrapper'); // ダッシュボード要素を取得
      //console.log(dashboardWrapper);
      const logWrapper = document.getElementById('logWrapper'); // ログラッパー要素を取得
      //console.log(logWrapper);
      const tabs = document.getElementById('tabBar').children; // タブバーの子要素を取得
      for (let i = 0; i < tabs.length; i++) {
        tabs[i].classList.remove('active'); // 子クラスからクラスを消去
      }
      document.getElementById(selectedTabId).classList.add('active'); // 選択されたタブにクラスを付与
      if (selectedTabId === 'dashboardTab') {
        dashboardWrapper.style.display = ''; // ダッシュボードを表示
        logWrapper.style.display = 'none'; // ログラッパーを非表示
      } else if (selectedTabId === 'logTab') {
        dashboardWrapper.style.display = 'none'; // ダッシュボードを非表示
        logWrapper.style.display = ''; // ログラッパーを表示
      }
    }
    changeTab('dashboardTab'); // 初期表示はダッシュボードタブ


    // ログ一時停止ボタンのイベントリスナー設定
    let paused = false; // 一時停止状態を示すフラグ
    const pauseButton = document.getElementById('pauseButton');
    pauseButton.addEventListener('click', () => { // 一時停止ボタンのクリックイベントリスナーを設定
      const pauseLog = document.getElementById('pauseLog');
      paused = !paused; // 一時停止状態をトグル
      pauseButton.classList.toggle('active'); // クラスの付与状態をトグル
      if (!paused) {
        const logDiv = document.querySelectorAll('#logWindow > div');
        logDiv.forEach((element) => {
          element.style.display = '';
        })
        const logSpans = document.querySelectorAll('#logWindow > div > span > span');
        logSpans.forEach((element) => {
          if (element.classList.contains('timestamp')) {
            element.style.display = showTimestamps ? '' : 'none';
          } else {
            element.style.display = '';
          }
        });
      }
    });


    // ログ消去ボタンのイベントリスナー設定
    const clearButton = document.getElementById('clearButton');
    clearButton.addEventListener('click', () => { // クリアボタンのクリックイベントリスナーを設定
      const logWindow = document.getElementById('logWindow'); // ログウィンドウ要素を取得
      logWindow.innerHTML = ''; // ログウィンドウの内容をクリア
    });


    // タイムスタンプのチェックボックスのイベントリスナーを設定
    let showTimestamps = true; // タイムスタンプの表示状態を示すフラグ
    const timestampsButton = document.getElementById('logTimestamps'); // タイムスタンプボタンの要素を取得
    timestampsButton.addEventListener('click', () => {
      timestampsButton.classList.toggle('active');
      const timestamps = document.querySelectorAll('.timestamp'); // timestampクラスを取得
      showTimestamps = !showTimestamps;
      timestamps.forEach(element => {
        if (showTimestamps) {
          element.style.display = 'inline'; // 表示
        }
        else {
          element.style.display = 'none'; // 非表示
        }
      });
    });


    // ログを表示するヘルパー関数
    const MAX_LOG_ENTRIES = 200;
    let lineBreak = true;
    function addLog(rawMsg) {
      const safeMsg = (typeof rawMsg === 'string') ? rawMsg : String(rawMsg); // undefinedやnullの対策
      const splitedMsg = rawMsg.split(/(\r\n|\r|\n)/); // 正規表現で分割
      const msg = splitedMsg.filter(Boolean);
      const logWindow = document.getElementById('logWindow'); // ログウィンドウ要素を取得

      msg.forEach((element) => {
        if (lineBreak) {    
      const now = new Date();
          const timestamp = document.createElement('span'); // タイムスタンプ用のspan要素を作成
      timestamp.textContent = `[${now.toLocaleTimeString()}:${now.getMilliseconds().toString().padStart(3, '0')}] `;
      timestamp.classList.add('timestamp');
      if (!showTimestamps) {
        timestamp.style.display = 'none';
      }
      
          const msgSpan = document.createElement('span');
          msgSpan.textContent = element; // ログメッセージを設定
      
      const lineSpan = document.createElement('span');
      lineSpan.appendChild(timestamp); // タイムスタンプを追加
          lineSpan.appendChild(msgSpan); // ログメッセージを追加 

      const logEntry = document.createElement('div'); // 新しいdiv要素を作成
      logEntry.appendChild(lineSpan); // 1行追加
      logWindow.insertBefore(logEntry, logWindow.firstChild); // ログウィンドウの先頭にログエントリを追加
          if (paused) {
            logEntry.style.display = 'none'; // 一時停止が有効なら非表示
          }
          lineBreak = false;
        }
        else if (element === '\r\n' || element === '\r' || element === '\n') {
          lineBreak = true;
        }
        else {
          const msgSpan = document.createElement('span');
          msgSpan.textContent = element; // ログメッセージを設定
          const firstLineSpan = document.querySelector('#logWindow > div > span');
          firstLineSpan.appendChild(msgSpan);
          if (paused) {
            msgSpan.style.display = 'none'; // 一時停止が有効なら非表示
          }
        }
      });

      while (logWindow.children.length > MAX_LOG_ENTRIES && !paused) { // ログの数がMAX_LOG_ENTRIES未満になるまで繰り返し
        logWindow.removeChild(logWindow.lastChild); // 最後の（一番古い）ログを消去
      }
    }

    
    function addLogLn(msg) {
      addLog(`${msg}\n`);
    }


    // ダッシュボードアイテムを更新する関数
    function updateDashbordItem(index, label, value) {
      if (document.getElementById(index) === null) {
        const dashboardWrapper = document.getElementById('dashboardWrapper'); // ダッシュボード要素を取得
        const itemDiv = document.createElement('div'); // 新しいdiv要素を作成
        itemDiv.id = index; // 数値のみのIDを付与

        const labelDiv = document.createElement('div'); // ラベル用のdiv要素を作成
        labelDiv.classList.add('label'); // labelクラスに追加
        labelDiv.id = `label${index}`; // label+数値のIDを付与

        const valueDiv = document.createElement('div'); // 値用のdiv要素を作成
        valueDiv.classList.add('value'); // valueクラスに追加
        valueDiv.id = `value${index}`; // value+数値のIDを付与

        itemDiv.appendChild(labelDiv); // ラベルdivをアイテムdivに追加
        itemDiv.appendChild(valueDiv); // 値divをアイテムdivに追加
        dashboardWrapper.appendChild(itemDiv); // アイテムdivをダッシュボードに追加
      }

      const itemDiv = document.getElementById(index);
      itemDiv.classList.add('update');
      setTimeout(() => {
        itemDiv.classList.remove('update');
      }, 200);
      const labelDiv = document.getElementById(`label${index}`);
      const valueDiv = document.getElementById(`value${index}`);
      labelDiv.textContent = label; // ラベルテキストを設定
      valueDiv.textContent = value; // 値テキストを設定

      addLogLn(`[Update] ${label} : ${value}`);
    }
    
    
    // ログ送信フォームのイベントリスナー設定
    const logForm = document.getElementById('logForm');
    logForm.addEventListener('submit', function(event) {
      event.preventDefault(); // フォームのデフォルトの送信動作を防止
      const logInput = document.getElementById('logInput'); // ログ入力要素を取得
      const msg = logInput.value.trim(); // 入力値を取得してトリム

      if (msg === '') return;

      const isIOS = /iPad|iPhone|iPod/.test(navigator.userAgent);
      // addLogLn(isIOS);

      if (isIOS || !ws || ws.readyState !== WebSocket.OPEN) {
        fetch(`/send?msg=${encodeURIComponent(msg)}`)
         .then(resp => {
            if (resp.ok) {
            logInput.value = ''; // 送信成功
          }
          else {
            addLogLn('HTTP Send Failed');
          }
        })
        .catch(err => addLogLn('HTTP Error: ' + err));
      }
      else {
          ws.send(msg); // WebSocketが接続されていればメッセージを送信
          // addLogLn(`Sent: ${msg}`); // 送信ログを追加
          logInput.value = ''; // 入力フィールドをクリア
      }
    });


    // ページ上部にジャンプするボタンのイベントリスナー
    const goToTopButton = document.getElementById('goToTopButton');
    goToTopButton.addEventListener('click', () => {
      // ページの一番上へ滑らかにスクロールする
      window.scrollTo({
        top: 0,
        behavior: 'smooth',
      });
    });


    // ページ上部にジャンプするボタンを表示する
    window.addEventListener('scroll', () => {
      console.log('scroll event');
      if (window.scrollY === 0 && goToTopButton.style.display === 'block') {
        goToTopButton.style.opacity = '0';
        setTimeout(() => {
          goToTopButton.style.display = 'none';
        }, 500);
      }
      else if (window.scrollY !== 0 && goToTopButton.style.display === 'none') {
        goToTopButton.style.display = 'block';
        goToTopButton.style.opacity = '0';
        setTimeout(() => {
          goToTopButton.style.opacity = '1';
        }, 1);
      }
    });

  </script>
</body>
</html>

)";