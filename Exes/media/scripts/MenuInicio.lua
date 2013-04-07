-- Fichero MenuInicio.lua

-- CARGA Y ACTIVACION DEL MENU EN LUA
if menuInicioWindow==nil then
	GUI.Windows:loadWindowLayout("Inicio.layout")
	
	menuInicioWindow = GUI.Windows:getWindow("Inicio")
	
	GUI.System:setGUISheet(menuInicioWindow)
	menuInicioWindow:setVisible(true)
	menuInicioWindow:activate()
	GUI.Mouse:show()

	--GESTION DE EVENTOS --Instalamos los eventos
	pantallaInicio = GUI.Windows:getWindow("Inicio")
	pantallaInicio:subscribeEvent("Clicked", "LimpiarPantalla")
	pantallaInicio:subscribeEvent("backspace", "LimpiarPantalla")
	pantallaInicio:subscribeEvent("enter", "LimpiarPantalla")
end