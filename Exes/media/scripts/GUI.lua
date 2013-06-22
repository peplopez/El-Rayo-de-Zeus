-- Tabla GUI por comodidad que apuntan a los distintos subsistemas.
GUI = {
	System = CEGUI.System:getSingleton(),
	Schemes = CEGUI.SchemeManager:getSingleton(),
	Fonts = CEGUI.FontManager:getSingleton(),
	Windows = CEGUI.WindowManager:getSingleton(),
	Images = CEGUI.ImagesetManager:getSingleton(),
	Mouse = CEGUI.MouseCursor:getSingleton(),
	Looknfeels = CEGUI.WidgetLookManager:getSingleton(),
	Logger = CEGUI.Logger:getSingleton(),
}

-- Schemes Initialization
GUI.Schemes:create("TaharezLook.scheme")
GUI.Schemes:create("OgreTray.scheme")

-- Fonts Initialization
GUI.Fonts:create("DejaVuSans-6.font")
GUI.Fonts:create("DejaVuSans-6bold.font")
GUI.Fonts:create("DejaVuSans-8.font")
GUI.Fonts:create("DejaVuSans-8bold.font")
GUI.Fonts:create("DejaVuSans-10.font")
GUI.Fonts:create("FairChar-30.font")
GUI.Fonts:create("Diogenes-10.font")
GUI.Fonts:create("Diogenes-14.font")
GUI.Fonts:create("Diogenes-24.font")
GUI.Fonts:create("Diogenes-24.font")
GUI.Fonts:create("Cantoria-8.font")
GUI.Fonts:create("Cantoria-10.font")
GUI.Fonts:create("Cantoria-14.font")

GUI.System:setDefaultMouseCursor("OgreTrayImages","MouseArrow")

--SManager:loadScript("Inicio")
--SManager:loadScript("Menu")
