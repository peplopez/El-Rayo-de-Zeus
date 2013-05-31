-- Fichero MenuSingle.lua

--Funcion de inicializacion/carga de un layout
function initMenuSingle()
	if menuSingle==nil then
		GUI.Windows:loadWindowLayout("MenuSingle.layout")
		menuSingle = GUI.Windows:getWindow("MenuSingle")
		
		--menuSingleStatusWindow = menuSingle:getChild("MenuSingle/Status")

	end
end


--Funcion que muestra una ventana del GUI
function showMenuSingle()
	if menuSingle ~= nil then
		GUI.System:setGUISheet(menuSingle)
		menuSingle:setVisible(true)
		menuSingle:activate()
		GUI.Mouse:show()
	end
end


--Funcion que oculta una ventana del GUI
function hideMenuSingle()
	if menuSingle ~= nil then
		GUI.Mouse:hide()
		menuSingle:deactivate()		
		menuSingle:setVisible(false)
	end
end


function asociarEventosMenuSingle()
	--GESTION DE EVENTOS --Instalamos los eventos
	if menuSingle ~= nil then
		menuSinglePlay = GUI.Windows:getWindow("MenuSingle/Play")
		menuSingleBack = GUI.Windows:getWindow("MenuSingle/Back")
	end
	
	--if menuSingle ~= nil then
	--	menuSingle:subscribeEvent("Clicked", "LimpiarPantalla")
	--end
	--menuMulti:subscribeEvent("Clicked", "LimpiarPantalla")
	--menuExit:subscribeEvent("Clicked", "LimpiarPantalla")
end

function reloadMenuSingle()
		if menuSingle ~= nil then	
			menuSingleStatusWindow:setProperty("UnifiedAreaRect","{{0.1,1},{0.75,0},{0.95,0},{0.85,0}}")
		end
end

function showError(msgError)
	--menuSingleStatusWindow:setProperty("AlwaysOnTop","True")
    --menuSingleStatusWindow:setProperty("Text",msgError)
	--menuSingleStatusWindow:setProperty("AlwaysOnTop","False")
end