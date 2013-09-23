-- Fichero MenuSingle.lua

--Funcion de inicializacion/carga de un layout
function initMenuSingle()
	if menuSingle==nil then
		GUI.Windows:loadWindowLayout("MenuSingle.layout")
		menuSingle = GUI.Windows:getWindow("MenuSingle")
		
		--menuSingleModelBox = menuSingle:getChild("MenuSingle/ModelBox")
		menuSingleNickBoxLabel = menuSingle:getChild("MenuSingle/NickBoxLabel")
		menuSingleNickBox = menuSingle:getChild("MenuSingle/NickBox")
		
		menuSingleColorBoxLabel = menuSingle:getChild("MenuSingle/ColorBoxLabel")
		menuSingleColorBox = menuSingle:getChild("MenuSingle/ColorBox")

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
			menuSingleProgress = GUI.Windows:getWindow("MenuSingle/Progreso")
			--menuSingleProgress:setVisible(true)
			--menuSingleProgress:setProperty("UnifiedAreaRect", "{{0.05,0},{0.78,0},{0.95,0},{0.8,0}}")
			
			--menuSingleModelBox:setProperty("UnifiedAreaRect", "{{0.6,0},{0.57,0},{0.95,0},{0.9,0}}")
			--menuSingleModelBox:setProperty("UnifiedMaxSize", "{{1,0},{1,0}}")
			
			menuSingleNickBoxLabel:setProperty("UnifiedAreaRect", "{{0.58,1},{0.4,0},{0.69,0},{0.45,0}}")
			menuSingleNickBox:setProperty("UnifiedAreaRect", "{{0.7,0},{0.4,0},{0.95,0},{0.45,0}}")
			
			menuSingleColorBoxLabel:setProperty("UnifiedAreaRect", "{{0.58,1},{0.47,0},{0.69,0},{0.52,0}}")
			menuSingleColorBox:setProperty("UnifiedAreaRect", "{{0.7,0},{0.47,0},{0.95,0},{0.56,0}}")
		end
end

function showError(msgError)
	--menuSingleStatusWindow:setProperty("AlwaysOnTop","True")
    --menuSingleStatusWindow:setProperty("Text",msgError)
	--menuSingleStatusWindow:setProperty("AlwaysOnTop","False")
end