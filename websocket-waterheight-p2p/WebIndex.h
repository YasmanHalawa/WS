const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <title>SIGABUT WEBSOCKET</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous" />
    <link rel="icon" href="data:," />
    <style>
      html {
        font-family: "Arial", sans-serif;
        display: inline-block;
        text-align: center;
        background-color: #f4f4f4;
        color: #333;
      }
      body {
        margin: 0;
        padding: 0;
      }
      h3,
      p {
        margin: 0.5rem 0;
      }
      p {
        font-size: 1rem;
      }
      h4 {
        font-size: 1rem;
        margin-bottom: 0.5rem;
      }
      .w3-light-grey,
      .w3-light-gray,
      .w3-hover-light-grey:hover,
      .w3-hover-light-gray:hover {
        color: #000 !important;
        background-color: #f1f1f1 !important;
        height: 20px; /* Menambah tinggi progress bar */
        border-radius: 15px;
        margin: 0 20px; /* Menambahkan marging horizontal */
      }

      .w3-container.w3-blue {
        background: linear-gradient(to right, #1b78e2, #005f6b); /* Menambahkan gradient warna */
        height: 100%;
        border-radius: 15px; /* Membuat sudut progress bar lebih halus */
        transition: width 0.4s ease; /* Menambahkan animasi perubahan */
      }

      #progress-bar {
        height: 20px;
        line-height: 20px;
        color: white;
        text-align: center;
        font-weight: bold;
        border-radius: 15px;
        transition: width 0.4s ease;
        background: linear-gradient(to right, #1b78e2, #005f6b);
         
      }

      .topnav {
        overflow: hidden;
        background-color: #005f6b;
        color: white;
        padding: 1rem;
        font-size: 1.5rem;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      }
      .content {
        padding: 20px;
      }
      .card {
        background-color: #ffffff;
        box-shadow: 0px 4px 12px rgba(0, 0, 0, 0.1);
        border-radius: 15px;
        overflow: hidden;
        transition: transform 0.2s;
      }
      .card:hover {
        transform: scale(1.02);
      }
      .card.header {
        background-color: #005f6b;
        color: white;
        padding: 10px 15px;
      }
      .cards {
        max-width: 700px;
        margin: 0 auto;
        display: grid;
        grid-gap: 2rem;
        grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
      }
      .reading {
        font-size: 1.5rem;
        font-weight: bold;
      }
      .humidityColor {
        color: #1b78e2;
      }
      .statusreadColor {
        color: #702963;
        font-size: 0.9rem;
      }
      /* ----------------------------------- Toggle Switch */
      .switch {
        position: relative;
        display: inline-block;
        width: 50px;
        height: 24px;
        margin: 10px 0;
      }
      .switch input {
        display: none;
      }
      .sliderTS {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #d3d3d3;
        transition: 0.4s;
        border-radius: 34px;
      }
      .sliderTS:before {
        position: absolute;
        content: "";
        height: 16px;
        width: 16px;
        left: 4px;
        bottom: 4px;
        background-color: #f7f7f7;
        transition: 0.4s;
        border-radius: 50%;
      }
      input:checked + .sliderTS {
        background-color: #005f6b;
      }
      input:checked + .sliderTS:before {
        transform: translateX(26px);
      }
      .sliderTS:after {
        content: "OFF";
        color: white;
        display: block;
        position: absolute;
        transform: translate(-50%, -50%);
        top: 50%;
        left: 70%;
        font-size: 10px;
        font-family: Verdana, sans-serif;
      }
      input:checked + .sliderTS:after {
        left: 25%;
        content: "ON";
      }
      input:disabled + .sliderTS {
        opacity: 0.3;
        cursor: not-allowed;
        pointer-events: none;
      }
      /* ----------------------------------- Tooltip */
      .tooltip {
        position: relative;
        display: inline-block;
      }
      .tooltip .tooltiptext {
        visibility: hidden;
        width: 140px;
        background-color: #005f6b;
        color: #fff;
        text-align: center;
        border-radius: 6px;
        padding: 5px 0;
        position: absolute;
        z-index: 1;
        bottom: 125%;
        left: 50%;
        margin-left: -70px;
        opacity: 0;
        transition: opacity 0.3s, visibility 0.3s;
      }
      .tooltip .tooltiptext::after {
        content: "";
        position: absolute;
        top: 100%;
        left: 50%;
        margin-left: -5px;
        border-width: 5px;
        border-style: solid;
        border-color: #005f6b transparent transparent transparent;
      }
      .tooltip:hover .tooltiptext {
        visibility: visible;
        opacity: 1;
      }
    </style>
  </head>

  <body>
    <div class="topnav">
      <h3>Sigabut Websocket</h3>
    </div>

    <br />

    <!-- __ KODINGAN DI WEBSCOKET ____________________________________________________________________________________________ -->
    <!-- __ DISPLAYS MONITORING ____________________________________________________________________________________________ -->
    <div class="content">
      <div class="cards">
        <!-- == ESP32 Node Monitoring =========================================================== -->
        <div class="card">
          <div class="card header">
            <h3 style="font-size: 1.2rem">Sumur <span id="node_order"></span></h3>
          </div>

          <h4 class="humidityColor"><i class="fas fa-tint"></i> Kedalaman Air</h4>
          <p class="humidityColor">
            <span class="reading"><span id="height"></span> cm</span>
          </p>

          <br />
          <div class="w3-light-grey w3-round-xlarge">
            <div id="progress-bar" class="w3-container w3-blue w3-round-xlarge" aria-valuenow="0" aria-valuemin="0" style="width: 0%"></div>
          </div>
          <!-- Shows other information received from node. ************************ -->
          <p class="statusreadColor">
            <b><span>Waktu penerimaan data terbaru : </span><span id="LTRD_node"></span></b>
          </p>
          <!-- *********************************************************************** -->
        </div>
        <!-- ======================================================================================================= -->
      </div>
    </div>
    <!-- ___________________________________________________________________________________________________________________________________ -->

    <p id="Show_Info" style="font-size: 0.9rem; color: #00878f"></p>

    <script>
      //------------------------------------------------------------
      var Timer = setInterval(myTimer, 100);
      var TimerShowInfo = setInterval(myTimerShowInfo, 100);
      var count_to_Ctrl_TglBtn = 0;
      var count_to_Show_Info = 0;
      var count_to_Show_Info_no_Data_is_coming = 0;
      let change_TglBtn_Disable = false;
      let start_Show_Info_For_First_Time = true;
      //------------------------------------------------------------

      //------------------------------------------------------------
      document.getElementById("height").innerHTML = "...";
      document.getElementById("node_order").innerHTML = "...";
      document.getElementById("LTRD_node").innerHTML = "...";

      document.getElementById("Show_Info").innerHTML = "Please wait a while to receive data from Node.";
      //------------------------------------------------------------

      //------------------------------------------------------------ Processes the data received from the ESP32 Master.
      if (!!window.EventSource) {
        var source = new EventSource("/events");

        source.addEventListener(
          "open",
          function (e) {
            console.log("Events Connected");
          },
          false
        );

        source.addEventListener(
          "error",
          function (e) {
            if (e.target.readyState != EventSource.OPEN) {
              console.log("Events Disconnected");
            }
          },
          false
        );

        source.addEventListener(
          "message",
          function (e) {
            console.log("message", e.data);
          },
          false
        );

        source.addEventListener(
          "allDataJSON",
          function (e) {
            console.log("allDataJSON", e.data);

            var today = new Date();
            var time = leading_zeros(today.getHours()) + ":" + leading_zeros(today.getMinutes()) + ":" + leading_zeros(today.getSeconds());

            count_to_Show_Info_no_Data_is_coming = 0;
            if (document.getElementById("Show_Info").innerHTML == "No data comes from Node after 10 seconds from the last time the Master received data.") {
              document.getElementById("Show_Info").innerHTML = "";
            }

            var obj = JSON.parse(e.data);

            document.getElementById("height").innerHTML = obj.height;
            document.getElementById("height").innerHTML = obj.height;
            let progressBar = document.getElementById("progress-bar");
            progressBar.style.width = `${obj.height}%`;
            progressBar.setAttribute("aria-valuenow", obj.height);
            progressBar.innerHTML = `${obj.height}%`;
            document.getElementById("node_order").innerHTML = obj.node_order;

            document.getElementById("LTRD_node").innerHTML = time;
          },
          false
        );
      }
      //------------------------------------------------------------

      function myTimer() {
        if (change_TglBtn_Disable == true) {
          count_to_Ctrl_TglBtn += 100;
          if (count_to_Ctrl_TglBtn > 1000) {
            count_to_Ctrl_TglBtn = 0;
            change_TglBtn_Disable = false;
          }
        }
      }

      //------------------------------------------------------------
      function myTimerShowInfo() {
        count_to_Show_Info_no_Data_is_coming += 100;
        if (count_to_Show_Info_no_Data_is_coming > 10000) {
          count_to_Show_Info_no_Data_is_coming = 0;

          document.getElementById("Show_Info").innerHTML = "No data comes from node after 10 seconds from the last time the Master received data.";
        }

        count_to_Show_Info += 100;
        if (count_to_Show_Info > 1500) {
          count_to_Show_Info = 0;

          if (start_Show_Info_For_First_Time == true) {
            start_Show_Info_For_First_Time = false;
            document.getElementById("Show_Info").innerHTML = "";
            return;
          }

          if (document.getElementById("Show_Info").innerHTML == "Sending Data to Node, Please Wait...") {
            document.getElementById("Show_Info").innerHTML = "Please wait a moment for Node to reply to the message.";
            return;
          }
          if (document.getElementById("Show_Info").innerHTML == "Please wait a moment for Node to reply to the message.") {
            document.getElementById("Show_Info").innerHTML = "";
            return;
          }
        }
      }
      //------------------------------------------------------------

      //------------------------------------------------------------
      function change_TglBtn_Checked(TglBtnID) {
        var chck = document.getElementById(TglBtnID);
        chck.checked = !chck.checked;
      }
      //------------------------------------------------------------

      //------------------------------------------------------------
      function leading_zeros(x) {
        return (x < 10 ? "0" : "") + x;
      }
      //------------------------------------------------------------
    </script>
  </body>
</html>
)=====";
