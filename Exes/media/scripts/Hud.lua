-- Fichero Hud.lua

-- Vars initialization showed in HUD

numberAltarsActivated = 0
numberEnemiesInBase = 0
PM = 100
lifeBase = lifeBaseMax

--Funcion de inicializacion/carga de un layout y sus ventanas y subventanas
function initHud()
	if hud==nil then
		GUI.Windows:loadWindowLayout("Hud.layout")
		hud = GUI.Windows:getWindow("Hud")
		
		NumberEnemyWindow = hud:getChild("Hud/NumEnemigos")
		PuntosMeritoWindow = hud:getChild("Hud/PuntosMerito")
		NumberAltaresActivatedWindow = hud:getChild("Hud/NumAltares")
		
		PersonajeWindow = hud:getChild("Hud/Personaje")
		
		NicknameWindow = hud:getChild("Hud/Nickname")

		--Rayo1Window = hud:getChild("Hud/RayoBase1")
		--Rayo2Window = hud:getChild("Hud/RayoBase2")
		--Rayo3Window = hud:getChild("Hud/RayoBase3")

		  RayoWindow = hud:getChild("Hud/RayoBase")
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

function showAltarsActivated(altarsactivated, altarsmax)
   NumberAltaresActivatedWindow:setText(altarsactivated..'/'..altarsmax)
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

function showEnemiesInBase(enemies)
   NumberEnemyWindow:setText(enemies)
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

function showPM(pm)
   PuntosMeritoWindow:setText(pm)
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
	   RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeEntire")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoActivado")
    elseif lifeBase == 2 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMedium")	
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 1 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMinimum")	
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 0 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMinimum")	
      --Rayo1Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    end
end

function showBaseLife()
	if lifeBase == 3 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeEntire")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoActivado")
    elseif lifeBase == 2 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMedium")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 1 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMinimum")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 0 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMinimum")
      --Rayo1Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    end
end

function reloadHud()
		hud:setProperty("UnifiedAreaRect","{{0,0},{0,0},{0.5,0},{0.2,0}}")
		PersonajeWindow:setProperty("UnifiedAreaRect","{{0.05,0},{0.065,0},{0.375,0},{0.9,0}}")
		RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeMedium")
		RayoWindow:setProperty("UnifiedAreaRect","{{0.82,0},{0.28,0},{0.92,0},{0.94,0}}")
		NicknameWindow:setProperty("UnifiedAreaRect","{{0.25,0},{0.05,0},{0.85,0},{0.35,0}}")
		--[[NumberEnemyWindow
		PuntosMeritoWindow
		NumberAltaresActivatedWindow
		Rayo1Window
		Rayo2Window
		Rayo3Window	]]
end

