-- MI primer LUA
--print "hola"
--a = 3
--a = "cadena"
a = "3e+3"
--print(a)

function saluda()
   print("Hola desde LUA")
end

function fact(n)
	r = 1
	for i=1,n do
		r = r * i
	end
	print("El factorial de"..n.."es"..r)
end

function factorial(n)
	if n == 1 then
		return 1
	else
		return n * factorial(n-1)
	end
end

function sumaUno(n)
  return n+1
end

tick()


-- Configuración
World = {
  type = "World",
  position = "0 0 0",
  model = "laberinto.mesh",
  static = true,
}

Camera = {
  type = "Camera",
  distance = 20,
  height = 13,
  targetDistance = 10,
  targetHeight = 7,
}

Player = {
  position = "0 0 0",
  orientation = 180,
  life = 100.0,
  model = "marine.mesh",
  speed = 0.05,
  defaultAnimation = "idle",
}

config = {
  resX = 0,
  resY = 0,
}

config.resX, config.resY = getResolutionLUA()

hud = {
  width = config.resX / 6,
  height = config.resY / 6,
}

hud.x = config.resX - hud.width * 1.5
hud.y = config.resY - hud.height * 1.5

-- Lo probamos
print(config.resX, config.resY)
print(hud.x, hud.y)

print("resX: "..getResX())
print("resY: "..getResY())