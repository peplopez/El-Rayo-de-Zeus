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

GUI.Schemes:create("TaharezLook.scheme")
GUI.Schemes:create("OgreTray.scheme")

GUI.Fonts:create("DejaVuSans-10.font")
GUI.Fonts:create("FairChar-30.font")
--[[
GUI.Fonts:create("Batang-26.font")
GUI.Fonts:create("handshop-30.font")
]]

GUI.System:setDefaultMouseCursor("OgreTrayImages","MouseArrow")

--SManager:loadScript("Inicio")
--SManager:loadScript("Menu")
