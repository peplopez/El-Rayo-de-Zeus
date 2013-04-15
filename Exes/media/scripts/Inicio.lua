-- Fichero Inicio.lua

--Funcion de inicializacion/carga de un layout
function initMenuInicio()
	if menuInicioWindow==nil then
		GUI.Windows:loadWindowLayout("Inicio.layout")
		menuInicioWindow = GUI.Windows:getWindow("Inicio")
	end
end


--Funcion que muestra una ventana del GUI
function showMenuInicio()
	if menuInicioWindow ~= nil then
		GUI.System:setGUISheet(menuInicioWindow)
		menuInicioWindow:setVisible(true)
		menuInicioWindow:activate()
		GUI.Mouse:show()
	end
end


--Funcion que oculta una ventana del GUI
function hideMenuInicio()
	if menuInicioWindow ~= nil then
		GUI.Mouse:hide()
		menuInicioWindow:deactivate()		
		menuInicioWindow:setVisible(false)
	end
end