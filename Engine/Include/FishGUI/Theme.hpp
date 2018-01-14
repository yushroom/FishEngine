    struct Theme {
        int mStandardFontSize                 = 16;
        int mButtonFontSize                   = 20;
        int mTextBoxFontSize                  = 20;
        int mWindowCornerRadius               = 2;
        int mWindowHeaderHeight               = 30;
        int mWindowDropShadowSize             = 10;
        int mButtonCornerRadius               = 2;
        float mTabBorderWidth                 = 0.75f;
        int mTabInnerMargin                   = 5;
        int mTabMinButtonWidth                = 20;
        int mTabMaxButtonWidth                = 160;
        int mTabControlWidth                  = 20;
        int mTabButtonHorizontalPadding       = 10;
        int mTabButtonVerticalPadding         = 2;
        
        NVGcolor mDropShadow                       = Color(0, 128);
        NVGcolor mTransparent                      = Color(0, 0);
        NVGcolor mBorderDark                       = Color(29, 255);
        NVGcolor mBorderLight                      = Color(92, 255);
        NVGcolor mBorderMedium                     = Color(35, 255);
        NVGcolor mTextColor                        = Color(255, 160);
        NVGcolor mDisabledTextColor                = Color(255, 80);
        NVGcolor mTextColorShadow                  = Color(0, 160);
        NVGcolor mIconColor                        = mTextColor;
        
        NVGcolor mButtonGradientTopFocused         = Color(64, 255);
        NVGcolor mButtonGradientBotFocused         = Color(48, 255);
        NVGcolor mButtonGradientTopUnfocused       = Color(74, 255);
        NVGcolor mButtonGradientBotUnfocused       = Color(58, 255);
        NVGcolor mButtonGradientTopPushed          = Color(41, 255);
        NVGcolor mButtonGradientBotPushed          = Color(29, 255);
        
        /* Window-related */
        NVGcolor mWindowFillUnfocused              = Color(43, 230);
        NVGcolor mWindowFillFocused                = Color(45, 230);
        NVGcolor mWindowTitleUnfocused             = Color(220, 160);
        NVGcolor mWindowTitleFocused               = Color(255, 190);
        
        NVGcolor mWindowHeaderGradientTop          = mButtonGradientTopUnfocused;
        NVGcolor mWindowHeaderGradientBot          = mButtonGradientBotUnfocused;
        NVGcolor mWindowHeaderSepTop               = mBorderLight;
        NVGcolor mWindowHeaderSepBot               = mBorderDark;
        
        NVGcolor mWindowPopup                      = Color(50, 255);
        NVGcolor mWindowPopupTransparent           = Color(50, 0);
    };