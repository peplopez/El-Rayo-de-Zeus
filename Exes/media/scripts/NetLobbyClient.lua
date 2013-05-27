-- Fichero NetLobbyClient.lua

--Funcion de inicializacion/carga de un layout
function initNetLobbyClient()
	if netLobbyClientWindow==nil then
		GUI.Windows:loadWindowLayout("NetLobbyClient.layout")
		netLobbyClientWindow = GUI.Windows:getWindow("NetLobbyClient")
		IPBoxLabel = netLobbyClientWindow:getChild("NetLobbyClient/IPBoxLabel")
		IPBox = netLobbyClientWindow:getChild("NetLobbyClient/IPBox")
		NickBoxLabel = netLobbyClientWindow:getChild("NetLobbyClient/NickBoxLabel")
		NickBox = netLobbyClientWindow:getChild("NetLobbyClient/NickBox")
		ModelBoxLabel = netLobbyClientWindow:getChild("NetLobbyClient/ModelBoxLabel")
		ModelBox = netLobbyClientWindow:getChild("NetLobbyClient/ModelBox")
		ColorBoxLabel = netLobbyClientWindow:getChild("NetLobbyClient/ColorBoxLabel")
		ColorBox = netLobbyClientWindow:getChild("NetLobbyClient/ColorBox")		
		Connect = netLobbyClientWindow:getChild("NetLobbyClient/Connect")
		Status = netLobbyClientWindow:getChild("NetLobbyClient/Status")
		Back = netLobbyClientWindow:getChild("NetLobbyClient/Back")
	end
end


--Funcion que muestra una ventana del GUI
function showNetLobbyClient()
	if netLobbyClientWindow ~= nil then
		GUI.System:setGUISheet(netLobbyClientWindow)
		netLobbyClientWindow:setVisible(true)
		netLobbyClientWindow:activate()
		GUI.Mouse:show()
	end
end


--Funcion que oculta una ventana del GUI
function hideNetLobbyClient()
	if netLobbyClientWindow ~= nil then
		GUI.Mouse:hide()
		netLobbyClientWindow:deactivate()		
		netLobbyClientWindow:setVisible(false)
	end
end


function asociarEventosNetLobbyClient()
	--GESTION DE EVENTOS --Instalamos los eventos
	if netLobbyClientWindow ~= nil then
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

function reloadNetLobbyClient()

		IPBoxLabel:setProperty("UnifiedAreaRect","{{0.6,0},{0.43,0},{0.69,0},{0.48,0}}")
		IPBox:setProperty("UnifiedAreaRect","{{0.7,0},{0.43,0},{0.95,0},{0.48,0}}")
		
		NickBoxLabel:setProperty("UnifiedAreaRect","{{0.6,1},{0.5,0},{0.69,0},{0.55,0}}")
		NickBox:setProperty("UnifiedAreaRect","{{0.7,0},{0.5,0},{0.95,0},{0.55,0}}")
		
		ModelBoxLabel:setProperty("UnifiedAreaRect","{{0.6,1},{0.57,0},{0.69,0},{0.62,0}}")
		
		ModelBox:setProperty("UnifiedAreaRect","{{0.1,0},{0.3,0},{0.95,0},{0.92,0}}")
		ModelBox:setProperty("UnifiedAreaRect","{{0.7,0},{0.57,0},{0.95,0},{0.8,0}}")
		ModelBox:setProperty("UnifiedMaxSize","{{1,0},{1,0}}")
		
		ColorBox:setProperty("UnifiedAreaRect","{{0.7,0},{0.64,0},{0.95,0},{0.85,0}}")
		
		Connect:setProperty("UnifiedAreaRect","{{0.80,0},{0.9,0},{0.95,0},{0.95,0}}")
		Status:setProperty("UnifiedAreaRect","{{0.2,1},{0.7,0},{0.8,0},{0.8,0}}")
		Back:setProperty("UnifiedAreaRect","{{0.05,0},{0.9,0},{0.2,0},{0.95,0}}")
		
end