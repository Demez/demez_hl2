"Resource/UI/BaseModUI/LoadGame.res"
{
	"LoadGame" //title = #L4D360UI_ChooseCampaign
	{
		"ControlName"			"Frame"
		"fieldName"				"LoadGame"
		"xpos"				"0"
		"ypos"				"0"
		"wide"				"f0"
		"tall"				"f0"
		"autoResize"				"0"
		"pinCorner"					"0"
		"visible"					"1"
		"enabled"					"1"
		"tabPosition"				"0"
	}
	"Title"
	{
		"fieldName"		"Title"
		"xpos"		"c-266"
		"ypos"		"51"
		"wide"		"150"
		"tall"		"19"
		"zpos"		"5"
		"font"		"DefaultExtraLarge"
		"textAlignment"		"west"
		"ControlName"		"Label"
		"labelText"		"#HL2PORTUI_Titles_LoadGame"
		"fgcolor_override"		"224 224 224 255"
	}
	"listpanel_loadgame"
	{
		"ControlName"		"PanelListPanel"
		"fieldName"		"listpanel_loadgame"
		"xpos"		"c-222"
		"ypos"		"82"
		"wide"		"400"
		"tall"		"300"
		"autoResize"		"3"
		"pinCorner"		"0"
		"visible"		"1"
		"enabled"		"1"
		"tabPosition"		"1"
	}
	"BtnDone" [$WIN32]
	{
		"ControlName"			"CNB_Button"
		"fieldName"				"BtnDone"
		"xpos"		"c-264"
		"ypos"		"r23"
		"wide"		"117"
		"tall"		"27"
		"zpos"		"1"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"labelText"				"#HL2PORTUI_Button_Load_Caps"
		"command"				"loadsave"
		"textAlignment"		"center"
		"font"		"DefaultMedium"
		"fgcolor_override"		"113 142 181 255"
	}
	"Delete"
	{
		"ControlName"		"CNB_Button"
		"fieldName"		"Delete"
		"xpos"		"r-106"
        "ypos"		"r23"
		"wide"		"117"
		"tall"		"27"
		"zpos"		"1"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"labelText"		"#GameUI_Delete"
		"textAlignment"		"center"
		"Command"		"Delete"
		"textAlignment"		"center"
		"font"		"DefaultMedium"
		"fgcolor_override"		"113 142 181 255"
	}
	"BtnCancel" [$WIN32]
	{
		"ControlName"			"CNB_Button"
		"fieldName"				"BtnCancel"
		"xpos"		"c-127"
		"ypos"		"r23"
		"wide"		"117"
		"tall"		"27"
		"zpos"		"1"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"labelText"				"#L4D360UI_Cancel_Caps"
		"command"				"Back"
		"textAlignment"		"center"
		"font"		"DefaultMedium"
		"fgcolor_override"		"113 142 181 255"
	}
}