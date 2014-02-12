var JSGlobal = {};

$(document).ready(function() {
  
  // Settings menu
  $("#settings-player").click(function() {
    $("#settings-menu").css("display", "none");
    $("#player-settings-menu").css("display", "table");
  } );
  $("#settings-graphics").click(function() {
    $("#settings-menu").css("display", "none");
    $("#graphics-settings-menu").css("display", "table");
  } );
  $("#settings-sound").click(function() {
    ShowError("Not yet implemented! D:");
  } );
  $("#settings-controls").click(function() {
    $("#settings-menu").css("display", "none");
    $("#controls-settings-menu").css("display", "table");
  } );
  $("#settings-back").click(function() {
    $("#settings-menu").css("display", "none");
    $("#ingame-menu").css("display", "table");
  } );
  // Player settings
  JSGlobal.LoadSettings = function () {
    var settings = Menu.RequestSettings();
		$("#controls-settings-wrapper > DIV").remove();
		
    for(var key in settings)
		{
      if(key == "settings-fullscreen")
        $("input[name=settings-fullscreen]").filter("[value="+settings[key]+"]").prop("checked", true);
      else if(key == "settings-resolution-list")
      {
        $("#settings-resolution").empty();
        eval(settings[key]).forEach(function(resolution) {
          $("#settings-resolution").append("<option value='"+resolution+"'>"+resolution+"</option>");
        });
        $("#settings-resolution").val(settings["settings-resolution"]);
      }
			else if(key.search("settings-key-") > -1)
			{
				$("#controls-settings-wrapper").append('<div><div class="inline-150px">'+key.substr(key.search("settings-key-"))+'</div><input id="'+key+'" value="'+settings[key]+'" class="controls-settings-keybind"/></div>');
			}
      else
        $("#"+key).val(settings[key]);
    }
		
		$(".controls-settings-keybind").focus(function() {
			$(this).css("border", "1px solid yellow");
			Menu.FocusBind($(this).id());
		} );
		$(".controls-settings-keybind").blur(function() {
			$(this).css("border", "0");
			Menu.UnfocusBind();
		} );
  }
  $("#player-settings-back").click(function() {
    $("#player-settings-menu").css("display", "none");
    $("div#settings-menu").css("display", "table");
    $("tr#settings-menu").css("display", "table-row");
    JSGlobal.LoadSettings();
  } );
  $("#player-settings-save").click(function() {
    var settings = {};
    settings["settings-player-name"] = $("#settings-player-name").val();
    Menu.SaveSettings(settings);
    $("#player-settings-menu").css("display", "none");
    $("div#settings-menu").css("display", "table");
    $("tr#settings-menu").css("display", "table-row");
  } );
  // Graphics settings
  $("#graphics-settings-back").click(function() {
    $("#graphics-settings-menu").css("display", "none");
    $("div#settings-menu").css("display", "table");
    $("tr#settings-menu").css("display", "table-row");
    JSGlobal.LoadSettings();
  } );
  $("#graphics-settings-save").click(function() {
    var settings = {};
    settings["settings-fullscreen"] = $("input[name=settings-fullscreen]:checked").val();
    settings["settings-resolution"] = $("#settings-resolution").val();
    Menu.SaveSettings(settings);
    $("#graphics-settings-menu").css("display", "none");
    $("div#settings-menu").css("display", "table");
    $("tr#settings-menu").css("display", "table-row");
  } );
  // Controls settings
  $("#controls-settings-back").click(function() {
    $("#controls-settings-menu").css("display", "none");
    $("div#settings-menu").css("display", "table");
    $("tr#settings-menu").css("display", "table-row");
    JSGlobal.LoadSettings();
  } );
  $("#controls-settings-save").click(function() {
    var settings = {};
		$(".controls-settings-keybind").each(function() {
			settings[$(this).prop('id')] = $(this).val();
		} );
    Menu.SaveSettings(settings);
    $("#controls-settings-menu").css("display", "none");
    $("div#settings-menu").css("display", "table");
    $("tr#settings-menu").css("display", "table-row");
  } );
} );