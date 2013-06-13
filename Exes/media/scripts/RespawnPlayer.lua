-- Fichero RespawnPlayer.lua

--Funcion de inicializacion/carga de un layout
function initRespawn()
	if respawnWindow==nil then
		GUI.Windows:loadWindowLayout("RespawnPlayer.layout")
		respawnWindow = GUI.Windows:getWindow("RespawnPlayer")
		
		TitleWindow = respawnWindow:getChild("RespawnPlayer/Texto")
		TimeWindow = respawnWindow:getChild("RespawnPlayer/Time")		
	end
end


--Funcion que muestra una ventana del GUI
function showRespawn()
	if respawnWindow ~= nil then
		GUI.System:setGUISheet(respawnWindow)
		respawnWindow:setVisible(true)
		respawnWindow:activate()
		GUI.Mouse:hide()
	end
end


--Funcion que oculta una ventana del GUI
function hideRespawn()
	if respawnWindow ~= nil then
		--GUI.Mouse:show()
		respawnWindow:deactivate()		
		respawnWindow:setVisible(false)
	end
end

function updateTime(txtTime)
	TimeWindow:setText(txtTime)
end

function reloadRespawnPlayer()

		respawnWindow:setProperty("UnifiedAreaRect","{{0,0},{0,0},{1,0},{1,0}}")
		
		TitleWindow:setProperty("UnifiedAreaRect","{{0.2,0},{0.4,0},{0.7,0},{0.6,0}}")
		TitleWindow:setProperty("Font","Diogenes-24")
		
		TimeWindow:setProperty("UnifiedAreaRect","{{0.75,0},{0.4,0},{0.85,0},{0.6,0}}")
		TimeWindow:setProperty("Font","Diogenes-24")

end