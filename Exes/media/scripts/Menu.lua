-- Fichero Menu.lua

--Funcion de inicializacion/carga de un layout
function initMenu()
	if menuWindow==nil then
		GUI.Windows:loadWindowLayout("Menu.layout")
		menuWindow = GUI.Windows:getWindow("Menu")
	end
end


--Funcion que muestra una ventana del GUI
function showMenu()
	if menuWindow ~= nil then
		GUI.System:setGUISheet(menuWindow)
		menuWindow:setVisible(true)
		menuWindow:activate()
		GUI.Mouse:show()
	end
end


--Funcion que oculta una ventana del GUI
function hideMenu()
	if menuWindow ~= nil then
		GUI.Mouse:hide()
		menuWindow:deactivate()		
		menuWindow:setVisible(false)
	end
end


function asociarEventosMenu()
	--GESTION DE EVENTOS --Instalamos los eventos
	if menuWindow ~= nil then
		menuSingle = GUI.Windows:getWindow("Menu/Single")
		menuMulti = GUI.Windows:getWindow("Menu/Multi")
		menuExit = GUI.Windows:getWindow("Menu/Exit")
	end
	
	--if menuSingle ~= nil then
	--	menuSingle:subscribeEvent("Clicked", "LimpiarPantalla")
	--end
	--menuMulti:subscribeEvent("Clicked", "LimpiarPantalla")
	--menuExit:subscribeEvent("Clicked", "LimpiarPantalla")
end