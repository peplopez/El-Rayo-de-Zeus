-- Fichero Hud.lua

-- Vars initialization showed in HUD
numberAltarsMax = 11
numberEnemiesInBaseMax = 8
lifeBaseMax = 3

numberAltarsActivated = 0
numberEnemiesInBase = 0
PM = 100
incPM = 50
lifeBase = lifeBaseMax

--Funcion de inicializacion/carga de un layout y sus ventanas y subventanas
function initHud()
	if hud==nil then
		GUI.Windows:loadWindowLayout("Hud.layout")
		hud = GUI.Windows:getWindow("Hud")
		
		NumberEnemyWindow = hud:getChild("Hud/NumEnemigos")
		PuntosMeritoWindow = hud:getChild("Hud/PuntosMerito")
		NumberAltaresActivatedWindow = hud:getChild("Hud/NumAltares")

		Rayo1Window = hud:getChild("Hud/RayoBase1")
		Rayo2Window = hud:getChild("Hud/RayoBase2")
		Rayo3Window = hud:getChild("Hud/RayoBase3")		
	end
end


--Funcion que muestra la ventana del HUD
function showHud()
	if hud ~= nil then
		GUI.System:setGUISheet(hud)
		hud:setVisible(true)
		hud:activate()
	end
end


--Funcion que oculta una ventana del GUI
function hideHud()
	if hud ~= nil then
		hud:deactivate()		
		hud:setVisible(false)
	end
end


function changeHudVisibility()
		if hud:isVisible() then
			hud:setVisible(false)
		else
			hud:setVisible(true)
		end
end


--Altars HUD functions
function incrementAltarsActivated()
  numberAltarsActivated = numberAltarsActivated+1
  --Control de rango
  if numberAltarsActivated > numberAltarsMax then
    numberAltarsActivated = numberAltarsMax
  end  
end

function decrementAltarsActivated()
  numberAltarsActivated = numberAltarsActivated-1
  --Control de rango  
  if numberAltarsActivated < 0 then
    numberAltarsActivated = 0
  end
end

function updateAltarsActivated()
   NumberAltaresActivatedWindow:setText(numberAltarsActivated..'/'..numberAltarsMax)
end

--NumberEnemies HUD functions
function incrementEnemiesInBase()
  numberEnemiesInBase = numberEnemiesInBase+1
    --Control de rango  
	if numberEnemiesInBase > numberEnemiesInBaseMax then
		numberEnemiesInBase = numberEnemiesInBaseMax
	end  
end

function decrementEnemiesInBase()
  numberEnemiesInBase = numberEnemiesInBase-1
    --Control de rango
	if numberEnemiesInBase < 0 then
		numberEnemiesInBase = 0
	end
end

function updateEnemiesInBase()
   NumberEnemyWindow:setText(numberEnemiesInBase)
end

-- pM HUD functions
function incrementPM()
  PM = PM+incPM
end

function decrementPM()
  PM = PM-incPM
end

function updatePM()
   PuntosMeritoWindow:setText(PM)
end

-- lifeBase HUD functions
function incrementBaseLife()
  lifeBase = lifeBase+1
  --Control de rango
	if lifeBase > lifeBaseMax then
		lifeBase = lifeBaseMax
	end	  
end

function decrementBaseLife()
  lifeBase = lifeBase-1
  --Control de rango
  if lifeBase < 0 then
	lifeBase = 0
  end  
end


function updateBaseLife()
	if lifeBase == 3 then
	  Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  Rayo3Window:setProperty("Image","set:HudBackground image:RayoActivado")
    elseif lifeBase == 2 then
	  Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 1 then
	  Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 0 then
      Rayo1Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    end
end

