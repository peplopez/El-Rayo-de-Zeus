-- Fichero Pause.lua

--Funcion de inicializacion/carga de un layout
function initPause()
	if pause==nil then
		root = GUI.Windows:getWindow("Root")
		--pause = root:getChild("Root/Pause")
		
		pause = GUI.Windows:getWindow("Root/Pause")
		
		GUI.System:setGUISheet(root)
		
		--TextoPause = GUI.Windows:getWindow("Root/Pause/Texto")
		
		--GUI.Windows:loadWindowLayout("Pause.layout")
		--pause = GUI.Windows:getWindow("Pause")
		--TitleWindow = pause:getChild("Pause/Texto")		
	end
end


--Funcion que muestra una ventana del GUI
function showPause()
	if pause ~= nil then
		--GUI.System:setGUISheet(pause)
		pause:setVisible(true)
		--pause:activate()
		--GUI.Mouse:hide()
	end
end


--Funcion que oculta una ventana del GUI
function hidePause()
	if pause ~= nil then
		--GUI.Mouse:show()
		--pause:deactivate()		
		pause:setVisible(false)
	end
end