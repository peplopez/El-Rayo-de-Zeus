-- Fichero GameOver.lua

--Funcion de inicializacion/carga de un layout
function initMenuGameOver()
	if menuGameOverWindow==nil then
		GUI.Windows:loadWindowLayout("GameOver.layout")
		menuGameOverWindow = GUI.Windows:getWindow("GameOver")
	end
end

--Funcion que muestra una ventana del GUI
function showMenuGameOver()
	if menuGameOverWindow ~= nil then
		GUI.System:setGUISheet(menuGameOverWindow)
		menuGameOverWindow:setVisible(true)
		menuGameOverWindow:activate()
		GUI.Mouse:show()
	end
end


--Funcion que oculta una ventana del GUI
function hideMenuGameOver()
	if menuGameOverWindow ~= nil then
		GUI.Mouse:hide()
		menuGameOverWindow:deactivate()		
		menuGameOverWindow:setVisible(false)
	end
end