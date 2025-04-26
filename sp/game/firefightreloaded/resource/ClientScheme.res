///////////////////////////////////////////////////////////
// Tracker scheme resource file
//
// sections:
//		Colors			- all the colors used by the scheme
//		BaseSettings	- contains settings for app to use to draw controls
//		Fonts			- list of all the fonts used by app
//		Borders			- description of all the borders
//
///////////////////////////////////////////////////////////
Scheme
{
	//////////////////////// COLORS ///////////////////////////
	// color details
	// this is a list of all the colors used by the scheme
	Colors
	{
		"White"				"255 255 255 255"
		"HL2Yellowish"			"255 160 0 255"
		"HL2Normal"				"255 208 64 255"
		"HL2Caution"			"255 48 0 255"
		"SMODBlueish"			"84 84 255 255"
		"SMODNormal"			"64 64 255 255"
		"SMODNormalDim"			"64 64 255 120"
		"SMODBright"			"128 128 255 255"
		"SMODCaution"			"255 84 255 255"
        "FRBlueish"			"30 120 255 255"
        "FRNormal"          "0 120 255 255"
        "FRNormalDim"       "0 120 255 120"
        "FRBright"          "48 133 219 255"
		
		// base colors
		"Orange"			"255 176 0 255"
		"OrangeDim"			"255 176 0 120"
		"LightOrange"		"188 112 0 128"
		
		"Red"				"192 28 0 140"
		"Black"				"0 0 0 196"
		"TransparentBlack"	"0 0 0 196"
		"TransparentLightBlack"	"0 0 0 90"

		"Blank"				"0 0 0 0"
		"ForTesting"		"255 0 0 32"
		"ForTesting_Magenta"	"255 0 255 255"
		"ForTesting_MagentaDim"	"255 0 255 120"
        
        "HL2_HUD_BG"    "0 0 0 120"
        "FR_HUD_BG"    "20 20 27 220"
	}
	
	///////////////////// BASE SETTINGS ////////////////////////
	//
	// default settings for all panels
	// controls use these to determine their settings
	BaseSettings
	{
		"FgColor"			"FRBlueish"
		"BgColor"			"FR_HUD_BG"

		"Panel.FgColor"			"FRBlueish"
		"Panel.BgColor"			"FR_HUD_BG"
		
		"BrightFg"		"FRBright"
		"BrightBg"		"FR_HUD_BG"

		"DamagedBg"			"180 0 0 200"
		"DamagedFg"			"180 0 0 230"
		"BrightDamagedFg"		"255 0 0 255"

		// weapon selection colors
		"SelectionNumberFg"		"FRBlueish"
		"SelectionTextFg"		"FRBlueish"
		"SelectionEmptyBoxBg" 	"FR_HUD_BG"
		"SelectionBoxBg" 		"FR_HUD_BG"
		"SelectionSelectedBoxBg" "FR_HUD_BG"
		
		"ZoomReticleColor"	"FRBlueish"

		// HL1-style HUD colors
		"Yellowish"			"FRBlueish"
		"Normal"			"FRNormal"
		"Caution"			"HL2Caution"

		// Top-left corner of the "Half-Life 2" on the main screen
		"Main.Title1.X"				"76"
		"Main.Title1.Y"				"145"
		"Main.Title1.Y_hidef"		"130"
		"Main.Title1.Color"	"255 255 255 255"

		// Top-left corner of secondary title e.g. "DEMO" on the main screen
		"Main.Title2.X"				"76"
		"Main.Title2.Y"				"190"
		"Main.Title2.Y_hidef"		"174"
		"Main.Title2.Color"	"255 255 255 200"

		// Top-left corner of the menu on the main screen
		"Main.Menu.X"			"53"
		"Main.Menu.X_hidef"		"76"
		"Main.Menu.Y"			"240"

		// Blank space to leave beneath the menu on the main screen
		"Main.BottomBorder"	"32"
		
		///HERE
				// vgui_controls color specifications
		Border.Bright					"FRBlueish"		// the lit side of a control
		Border.Dark						"FRBlueish"		// the dark/unlit side of a control
		Border.Selection				"Blank"				// the additional border color for displaying the default/selected button

		Button.TextColor				"FRNormal"
		Button.BgColor					"FR_HUD_BG"
		Button.ArmedTextColor			"FRNormal"
		Button.ArmedBgColor				"FRBlueish"
		Button.DepressedTextColor		"FRNormal"
		Button.DepressedBgColor			"FRBlueish"

		CheckButton.TextColor			"FRNormal"
		CheckButton.SelectedTextColor	"FRNormal"
		CheckButton.BgColor				"FR_HUD_BG"
		CheckButton.Border1  			"Border.Dark" 		// the left checkbutton border
		CheckButton.Border2  			"Border.Bright"		// the right checkbutton border
		CheckButton.Check				"FRNormal"				// color of the check itself

		ComboBoxButton.ArrowColor		"FRNormal"
		ComboBoxButton.ArmedArrowColor	"FRNormal"
		ComboBoxButton.BgColor			"FR_HUD_BG"
		ComboBoxButton.DisabledBgColor	"Blank"

		Frame.BgColor					"FR_HUD_BG"
		Frame.OutOfFocusBgColor			"FR_HUD_BG"
		Frame.FocusTransitionEffectTime	"0.0"	// time it takes for a window to fade in/out on focus/out of focus
		Frame.TransitionEffectTime		"0.0"	// time it takes for a window to fade in/out on open/close
		Frame.AutoSnapRange				"0"
		FrameGrip.Color1				"Blank"
		FrameGrip.Color2				"Blank"
		FrameTitleButton.FgColor		"Blank"
		FrameTitleButton.BgColor		"Blank"
		FrameTitleButton.DisabledFgColor	"Blank"
		FrameTitleButton.DisabledBgColor	"Blank"
		FrameSystemButton.FgColor		"Blank"
		FrameSystemButton.BgColor		"Blank"
		FrameSystemButton.Icon			""
		FrameSystemButton.DisabledIcon	""
		FrameTitleBar.TextColor			"FRNormal"
		FrameTitleBar.BgColor			"Blank"
		FrameTitleBar.DisabledTextColor	"FRNormal"
		FrameTitleBar.DisabledBgColor	"Blank"

		GraphPanel.FgColor				"FRNormal"
		GraphPanel.BgColor				"FR_HUD_BG"

		Label.TextDullColor				"FRNormal"
		Label.TextColor					"FRNormal"
		Label.TextBrightColor			"FRNormal"
		Label.SelectedTextColor			"FRNormal"
		Label.BgColor					"Blank"
		Label.DisabledFgColor1			"Blank"
		Label.DisabledFgColor2			"FRBlueish"

		ListPanel.TextColor					"FRNormal"
		ListPanel.BgColor					"FR_HUD_BG"
		ListPanel.SelectedTextColor			"Black"
		ListPanel.SelectedBgColor			"FRBlueish"
		ListPanel.SelectedOutOfFocusBgColor	"FRBlueish"
		ListPanel.EmptyListInfoTextColor	"FRNormal"

		Menu.TextColor					"FRNormal"
		Menu.BgColor					"FR_HUD_BG"
		Menu.ArmedTextColor				"FRNormal"
		Menu.ArmedBgColor				"FRBlueish"
		Menu.TextInset					"6"

		Chat.TypingText					"FRNormal"

		Panel.FgColor					"FRNormalDim"
		Panel.BgColor					"blank"

		ProgressBar.FgColor				"FRNormal"
		ProgressBar.BgColor				"FR_HUD_BG"

		PropertySheet.TextColor			"FRNormal"
		PropertySheet.SelectedTextColor	"FRNormal"
		PropertySheet.TransitionEffectTime	"0.25"	// time to change from one tab to another

		RadioButton.TextColor			"FRNormal"
		RadioButton.SelectedTextColor	"FRNormal"

		RichText.TextColor				"FRNormal"
		RichText.BgColor				"Blank"
		RichText.SelectedTextColor		"FRNormal"
		RichText.SelectedBgColor		"Blank"

		ScrollBarButton.FgColor				"FRNormal"
		ScrollBarButton.BgColor				"Blank"
		ScrollBarButton.ArmedFgColor		"FRNormal"
		ScrollBarButton.ArmedBgColor		"Blank"
		ScrollBarButton.DepressedFgColor	"FRNormal"
		ScrollBarButton.DepressedBgColor	"Blank"

		ScrollBarSlider.FgColor				"FRNormal"		// nob color
		ScrollBarSlider.BgColor				"Blank"		// slider background color

		SectionedListPanel.HeaderTextColor	"FRNormal"
		SectionedListPanel.HeaderBgColor	"Blank"
		SectionedListPanel.DividerColor		"Black"
		SectionedListPanel.TextColor		"FRNormal"
		SectionedListPanel.BrightTextColor	"FRNormal"
		SectionedListPanel.BgColor			"FR_HUD_BG"
		SectionedListPanel.SelectedTextColor			"FRNormal"
		SectionedListPanel.SelectedBgColor				"FRBlueish"
		SectionedListPanel.OutOfFocusSelectedTextColor	"FRNormal"
		SectionedListPanel.OutOfFocusSelectedBgColor	"255 255 255 32"

		Slider.NobColor				"FRNormal"
		Slider.TextColor			"FRNormal"
		Slider.TrackColor			"FRNormal"
		Slider.DisabledTextColor1	"FRBlueish"
		Slider.DisabledTextColor2	"FRBlueish"

		TextEntry.TextColor			"FRNormal"
		TextEntry.BgColor			"FR_HUD_BG"
		TextEntry.CursorColor		"FRNormal"
		TextEntry.DisabledTextColor	"FRNormal"
		TextEntry.DisabledBgColor	"Blank"
		TextEntry.SelectedTextColor	"Black"
		TextEntry.SelectedBgColor	"FRBlueish"
		TextEntry.OutOfFocusSelectedBgColor	"FRBlueish"
		TextEntry.FocusEdgeColor	"TransparentBlack"

		ToggleButton.SelectedTextColor	"FRNormal"

		Tooltip.TextColor			"White"
		Tooltip.BgColor				"TransparentBlack"

		TreeView.BgColor			"FR_HUD_BG"

		WizardSubPanel.BgColor		"Blank"
		
		"FgColor"		"FRNormal"
		"BgColor"		"FR_HUD_BG"
        
        "SteamDeckLoadingBar"			"FRNormal"
		"SteamDeckSpinner"				"FRNormal"
		"SteamDeckLoadingText"			"175 178 181 255"
	}

	//////////////////////// BITMAP FONT FILES /////////////////////////////
	//
	// Bitmap Fonts are ****VERY*** expensive static memory resources so they are purposely sparse
	BitmapFontFiles
	{
		// UI buttons, custom font, (256x64)
		"Buttons"		"materials/vgui/fonts/buttons_32.vbf"
	}

	
	//////////////////////// FONTS /////////////////////////////
	//
	// describes all the fonts
	Fonts
	{
		// fonts are used in order that they are listed
		// fonts are used in order that they are listed
		"DebugFixed"
		{
			"1"
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"antialias" "1"
			}
		}
		// fonts are used in order that they are listed
		"DebugFixedSmall"
		{
			"1"
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"antialias" "1"
			}
		}
		// fonts listed later in the order will only be used if they fulfill a range not already filled
		// if a font fails to load then the subsequent fonts will replace
		Default
		{
			"1"	[$X360]
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
			}
			"1"	[$WIN32]
			{
				"name"		"Verdana"
				"tall"		"9"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"768 1023"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1024 1199"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1200 10000"
				"additive"	"1"
			}
		}
		"DefaultSmall"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F"
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F"
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F"
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F"
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Arial"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
				"scanlines" "0"
			}
		}
		"DefaultVerySmall"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"0"
				"scanlines" "0"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Verdana"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
				"scanlines" "0"
			}
			"7"
			{
				"name"		"Arial"
				"tall"		"11"
				"range" 		"0x0000 0x00FF"
				"weight"		"0"
				"scanlines" "0"
			}
		}
        DefaultButtonPrompt
		{
			"1"	[$X360]
			{
				"name"		"PromptFont"
				"tall"		"12"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
			}
			"1"	[$WIN32]
			{
				"name"		"PromptFont"
				"tall"		"9"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1 599"
			}
			"2"
			{
				"name"		"PromptFont"
				"tall"		"12"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"PromptFont"
				"tall"		"14"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"768 1023"
			}
			"4"
			{
				"name"		"PromptFont"
				"tall"		"20"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1024 1199"
			}
			"5"
			{
				"name"		"PromptFont"
				"tall"		"24"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1200 10000"
				"additive"	"1"
			}
		}
		DefaultKillFeed
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"15"
				"weight"	"500"
				"antialias" "1"
			}
		}
		WeaponIconsKillFeed
		{
			"1"
			{
				"name"		"HL2MP"
				"tall"		"30"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsKillFeed2
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"30"
				"weight"	"2000"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsKillFeed3
		{
			"1"
			{
				"name"		"csd"
				"tall"		"30"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		AmmoIconsKillFeed
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"30"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		"MenuTitle"
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"22"
				"weight"	"500"
				"scanlines" "0"
				"antialias" "1"
			}
		}
		"MenuTitleSmall"
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"15"
				"weight"	"500"
				"scanlines" "0"
				"antialias" "1"
			}
		}
		WeaponIcons
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"64"
				"tall_hidef"	"58"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsSelected
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"64"
				"tall_hidef"	"58"
				"weight"	"0"
				"antialias" "1"
				"blur"		"4"
				"scanlines"	"2"
				"custom"	"1"
			}
		}
		WeaponIconsSmall
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"32"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		WeaponIconsCS
		{
			"1"
			{
				"name"		"cs"
				"tall"		"64"
				"tall_hidef"	"58"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsSelectedCS
		{
			"1"
			{
				"name"		"cs"
				"tall"		"64"
				"tall_hidef"	"58"
				"weight"	"0"
				"antialias" "1"
				"blur"		"4"
				"scanlines"	"2"
				"custom"	"1"
			}
		}
		WeaponIconsSmallCS
		{
			"1"
			{
				"name"		"cs"
				"tall"		"32"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		Crosshairs
		{
			"1"
			{
				"name"		"FRCrosshair"
				"tall"		"40"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "0"
				"custom"	"1"
				"yres"		"1 10000"
			}
		}
		QuickInfo
		{
			"1"	[$X360]
			{
				"name"		"HL2cross"
				"tall"		"57"
				"weight"	"0"
				"scanlines" "0"
				"antialias" 	"1"
				"custom"	"1"
			}
			"1"	[$WIN32]
			{
				"name"		"HL2cross"
				"tall"		"28" [!$OSX]
				"tall"		"50" [$OSX]
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1" [!$OSX]
			}
		}
		HudNumbers
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"26"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		SquadIcon
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"50"
				"weight"	"0"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersGlow
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"26"
				"weight"	"0"
				"blur"		"4"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersSmall
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"14"
				"weight"	"1000"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersGlowSmall
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"14"
				"weight"	"1000"
				"blur"		"4"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersSmallMoney
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"16"
				"weight"	"600"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersGlowSmallMoney
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"16"
				"weight"	"600"
				"blur"		"4"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersTimer
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"16"
				"weight"	"600"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersGlowTimer
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"16"
				"weight"	"600"
				"blur"		"4"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
        HudNumbersSmallHistory
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"1000"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudNumbersGlowSmallHistory
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"1000"
				"blur"		"4"
				"scanlines" "0"
				"antialias" "1"
				"custom"	"1"
			}
		}
		HudSelectionNumbers
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"11"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
			}
		}
		HudHintTextLarge
		{
			"1"	[$X360]
			{
				"bitmap"	"1"
				"name"		"Buttons"
				"scalex"	"1.0"
				"scaley"	"1.0"
				"scanlines" "0"
			}
			"1"	[$WIN32]
			{
				"name"		"Verdana" [!$OSX]
				"name"		"Helvetica Bold" [$OSX]
				"tall"		"14"
				"weight"	"1000"
				"scanlines" "0"
				"antialias" "1"
			}
		}
		HudHintTextSmall
		{
			"1"	[$WIN32]
			{
				"name"		"Verdana" [!$OSX]
				"name"		"Helvetica" [$OSX]
				"tall"		"11.5"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
			}
			"1"	[$X360]
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
			}
		}
		HudSelectionText
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"8"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"10"
				"weight"	"700"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"768 1023"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"16"
				"weight"	"900"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1024 1199"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"17"
				"weight"	"1000"
				"scanlines" "0"
				"antialias" "1"
				"yres"	"1200 10000"
			}
		}
		GameUIButtons
		{
			"1"	[$X360]
			{
				"bitmap"	"1"
				"name"		"Buttons"
				"scalex"	"0.63"
				"scaley"	"0.63"
				"scalex_hidef"	"1.0"
				"scaley_hidef"	"1.0"
			}
		}
		BudgetLabel
		{
			"1"
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"outline"	"1"
			}
		}
		DebugOverlay
		{
			"1"	[$WIN32]
			{
				"name"		"Courier New"
				"tall"		"14"
				"weight"	"400"
				"outline"	"1"
			}
			"1"	[$X360]
			{
				"name"		"Tahoma"
				"tall"		"18"
				"weight"	"200"
				"outline"	"1"
			}
		}
		"CloseCaption_Normal"
		{
			"1"
			{
				"name"		"Tahoma" [!$OSX]
				"name"		"Verdana" [$OSX]
				"tall"		"26" [!$OSX]
				"tall"		"24" [$OSX]
				"weight"	"500"
			}
		}
		"CloseCaption_Italic"
		{
			"1"
			{
				"name"		"Tahoma" [!$OSX]
				"name"		"Verdana Italic" [$OSX]
				"tall"		"26" [!$OSX]
				"tall"		"24" [$OSX]
				"weight"	"500"
				"italic"	"1"
			}
		}
		"CloseCaption_Bold"
		{
			"1"
			{
				"name"		"Tahoma" [!$OSX]
				"name"		"Verdana Bold" [$OSX]
				"tall"		"26" [!$OSX]
				"tall"		"24" [$OSX]
				"weight"	"900"
			}
		}
		"CloseCaption_BoldItalic"
		{
			"1"
			{
				"name"		"Tahoma" [!$OSX]
				"name"		"Verdana Bold Italic" [$OSX]
				"tall"		"26" [!$OSX]
				"tall"		"24" [$OSX]
				"weight"	"900"
				"italic"	"1"
			}
		}
		"CloseCaption_Small"
		{
			"1"
			{
				"name"		"Tahoma" [!$OSX]
				"name"		"Verdana" [$OSX]
				"tall"		"16" [!$OSX]
				"tall"		"14" [$OSX]
				"tall_hidef"	"24"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
			}
		}
		// this is the symbol font
		"Marlett"
		{
			"1"
			{
				"name"		"Marlett"
				"tall"		"14"
				"weight"	"0"
				"symbol"	"1"
			}
		}
		"Trebuchet24"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"24"
				"weight"	"900"
				"range"		"0x0000 0x007F"	//	Basic Latin
				"antialias" "1"
				"additive"	"1"
			}
		}
		"Trebuchet18"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"18"
				"weight"	"900"
			}
		}
		ClientTitleFont
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"34"
				"tall_hidef"	"48"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		ClientTitleFontMedium
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"24"
				"tall_hidef"	"38"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		ClientTitleFontSmall
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"15"
				"tall_hidef"	"27"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		ClientTitleFontHint
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"20"
				"tall_hidef"	"34"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		MenuTitleFont
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"34"
				"tall_hidef"	"48"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		MenuTitleFontMedium
		{
			"1"
			{
				"name"  "SMODGUI"
				"tall"			"20"
				"tall_hidef"	"38"
				"weight" "0"
				"additive" "0"
				"antialias" "1"
				"custom"	"1" [$OSX]
			}
		}
		CreditsLogo
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"128"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		CreditsText
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"20"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		CreditsOutroLogos
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"48"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		CreditsOutroText
		{
			"1"
			{
				"name"		"Verdana" [!$OSX]
				"name"		"Courier Bold" [$OSX]
				"tall"		"18"
				"weight"	"900"
				"antialias" "1"
			}
		}
		CenterPrintText
		{
			// note that this scales with the screen resolution
			"1"
			{
				"name"		"Trebuchet MS" [!$OSX]
				"name"		"Helvetica" [$OSX]
				"tall"		"18"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HDRDemoText
		{
			// note that this scales with the screen resolution
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"24"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		"AchievementNotification"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"14"
				"weight"	"900"
				"antialias" "1"
			}
		}
		"CommentaryDefault"
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"12"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"13"	[$WIN32]
				"tall"		"20"	[$X360]
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"768 1023"
				"antialias"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"20"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1024 1199"
				"antialias"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"24"
				"weight"	"900"
				"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
				"yres"	"1200 6000"
				"antialias"	"1"
			}
			"6"
			{
				"name"		"Verdana"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"900"
			}
			"7"
			{
				"name"		"Arial"
				"tall"		"12"
				"range" 		"0x0000 0x00FF"
				"weight"		"800"
			}
			
		}
		// ========================================================================================================================
		// New conditionals cannot be added for ClientScheme.res without engine access, so Steam Deck fonts use their own definitions.
		// ========================================================================================================================
		Default_Deck
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"16"
				"weight"	"700"
				"antialias" "1"
				"yres"	"1 599"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"22"
				"weight"	"700"
				"antialias" "1"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"26"
				"weight"	"900"
				"antialias" "1"
				"yres"	"768 1023"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"30"
				"weight"	"900"
				"antialias" "1"
				"yres"	"1024 1199"
			}
			"5" // Proportional - Josh
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		WeaponIcons_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"70"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsSelected_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"70"
				"weight"	"0"
				"antialias" "1"
				"blur"		"5"
				"scanlines"	"2"
				"additive"	"1"
				"custom"	"1"
			}
		}
		WeaponIconsSmall_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"36"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		FlashlightDeck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"46"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		Crosshairs_Deck
		{
			"1"
			{
				"name"		"FRCrosshair"
				"tall"		"40" [!$OSX]
				"weight"	"0"
				"antialias" "0"
				"additive"	"1"
				"custom"	"1"
				"yres"		"1 1439"
			}
			"2"
			{
				"name"		"FRCrosshair"
				"tall"		"80"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
				"yres"		"1440 3199"
			}
			"3"
			{
				"name"		"FRCrosshair"
				"tall"		"120"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
				"yres"		"3200 4799"
			}
			"4"
			{
				"name"		"FRCrosshair"
				"tall"		"17"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		QuickInfo_Deck
		{
			"1"
			{
				"name"		"HL2cross"
				"tall"		"36"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudNumbers_Deck
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"40"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudNumbersGlow_Deck
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"40"
				"weight"	"0"
				"blur"		"4"
				"scanlines" "2"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudNumbersSmall_Deck
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"26"
				"weight"	"1000"
				"additive"	"1"
				"antialias" "1"
				"custom"	"1"
			}
		}
		SquadIcon_Deck
		{
			"1"
			{
				"name"		"HalfLife2"
				"tall"		"40"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
				"custom"	"1"
			}
		}
		HudSelectionNumbers_Deck
		{
			"1"
			{
				"name"		"SMODGUI"
				"tall"		"16"
				"weight"	"700"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HudHintTextLarge_Deck
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"22"
				"weight"	"1000"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HudHintTextSmall_Deck
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"18"
				"weight"	"0"
				"antialias" "1"
				"additive"	"1"
			}
		}
		HudSelectionText_Deck
		{
			"1"
			{
				"name"		"Verdana"
				"tall"		"10"
				"weight"	"700"
				"antialias" "1"
				"yres"	"1 599"
				"additive"	"1"
			}
			"2"
			{
				"name"		"Verdana"
				"tall"		"14"
				"weight"	"700"
				"antialias" "1"
				"yres"	"600 767"
				"additive"	"1"
			}
			"3"
			{
				"name"		"Verdana"
				"tall"		"18"
				"weight"	"900"
				"antialias" "1"
				"yres"	"768 1023"
				"additive"	"1"
			}
			"4"
			{
				"name"		"Verdana"
				"tall"		"22"
				"weight"	"900"
				"antialias" "1"
				"yres"	"1024 1199"
				"additive"	"1"
			}
			"5"
			{
				"name"		"Verdana"
				"tall"		"9"
				"weight"	"900"
				"antialias" "1"
				"additive"	"1"
			}
		}
		"CloseCaption_Normal_Deck"
		{
			"1"
			{
				"name"		"Sans Regular"
				"tall"		"21" // 15
				"weight"	"500"
				"antialias"	"1"
			}
		}
		"CloseCaption_Italic_Deck"
		{
			"1"
			{
				"name"		"Sans Italic"
				"tall"		"21" // 15
				"weight"	"500"
				"italic"	"1"
				"antialias"	"1"
			}
		}
		"CloseCaption_Bold_Deck"
		{
			"1"
			{
				"name"		"Sans Bold"
				"tall"		"21" // 15
				"weight"	"900"
				"antialias"	"1"
			}
		}
		"CloseCaption_BoldItalic_Deck"
		{
			"1"
			{
				"name"		"Sand Bold Italic"
				"tall"		"21" // 15
				"weight"	"900"
				"italic"	"1"
				"antialias"	"1"
			}
		}
		//"CloseCaption_Small_Deck"
		//{
		//	"1"
		//	{
		//		"name"		"Tahoma"
		//		"tall"		"15"
		//		"weight"	"900"
		//		"range"		"0x0000 0x017F" //	Basic Latin, Latin-1 Supplement, Latin Extended-A
		//		"antialias"	"1"
		//	}
		//}
	}
	
	//////////////////////// CUSTOM FONT FILES /////////////////////////////
	//
	// specifies all the custom (non-system) font files that need to be loaded to service the above described fonts
	CustomFontFiles
	{
		"1"		"resource/HALFLIFE2.ttf"
		"2"		"resource/HL2crosshairs.ttf"
		"3"		"resource/HL2EP2.ttf"
		"4"		"resource/smodgui.ttf"
		"5"		"resource/hl2mp.ttf"
		"6"		"resource/cs.ttf"
		"7"		"resource/csd.ttf"
		"8"		"resource/FRCrosshair.ttf"
	}
}
