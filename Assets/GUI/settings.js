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
    $("#main-menu").css("display", "table-row");
  } );
	// Request setting data from the program and parse it
  JSGlobal.LoadSettings = function () {
    var settings = Menu.RequestSettings();
		$("#controls-settings-wrapper>DIV").remove();
		
    for(var key in settings)
		{
			// Radio button processing
			$("input[type='radio']").each(function(){
				if($(this).prop("name") == key && $(this).prop("value") == settings[key])
					$(this).prop("checked", true);
			});
			// Resolution list population
      if(key == "settings-resolution-list")
      {
        $("#settings-resolution").empty();
        eval(settings[key]).forEach(function(resolution) {
          $("#settings-resolution").append("<option value='"+resolution+"'>"+resolution+"</option>");
        });
        $("#settings-resolution").val(settings["settings-resolution"]);
      }
			// Keybinding processing
			else if(key.search("settings-key-") > -1)
			{
				var prefix = "settings-key-";
				$("#controls-settings-wrapper>input:first").before('<div><div class="inline-300px">'+key.substr(key.search(prefix)+prefix.length).replace('_',' ')+'</div><input id="'+key+'" value="'+settings[key]+'" class="controls-settings-keybind"/></div>');
			}
      else
        $("#"+key).val(settings[key]);
    }
		
		// Focus and unfocus events for keybindings
		$(".controls-settings-keybind").focus(function() {
			$(this).css("border", "1px solid #999999");
			Menu.FocusBind($(this).prop('id'));
		} );
		$(".controls-settings-keybind").blur(function() {
			$(this).css("border", "0");
			Menu.UnfocusBind();
		} );
		$(".controls-settings-keybind").css("border", "0");
  }
  // Player settings
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
    Menu.ChangeName(true);
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
    settings["settings-glow"] = $("input[name=settings-glow]:checked").val();
    settings["settings-grass"] = $("input[name=settings-grass]:checked").val();
    settings["settings-shadows"] = $("input[name=settings-shadows]:checked").val();
    settings["settings-water"] = $("input[name=settings-water]:checked").val();
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

	$("input[type=radio]").css('display', 'none');
	$("input[type=radio]").next("label").click(function(){
		$(this).prev().prop("checked", true);
	});
} );