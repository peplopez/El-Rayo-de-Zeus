-- Fichero MenuPrincipal.lua

-- CARGA Y ACTIVACION DEL MENU EN LUA
if menuWindow==nil then
	GUI.Windows:loadWindowLayout("Menu.layout")
	
	menuWindow = GUI.Windows:getWindow("Menu")
	
	GUI.System:setGUISheet(menuWindow)
	menuWindow:setVisible(true)
	menuWindow:activate()
	GUI.Mouse:show()

	--GESTION DE EVENTOS --Instalamos los eventos
	menuSingle = GUI.Windows:getWindow("Menu/Single")
	menuMulti = GUI.Windows:getWindow("Menu/Multi")
	menuExit = GUI.Windows:getWindow("Menu/Exit")
	
	menuSingle:subscribeEvent("Clicked", "LimpiarPantalla")
	menuMulti:subscribeEvent("Clicked", "LimpiarPantalla")
	menuExit:subscribeEvent("Clicked", "LimpiarPantalla")
	
end