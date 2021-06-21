//Code for detecting a drawn rithmatic line
using Godot;
using System;
using System.Collections.Generic;
/*
Code for the drawing surface, which tracks mouse location and draws while mouse is down.
Instantiates the type of drawn ritmatic line that fits best.
*/
public class DrawingSurface : Node2D{
    private bool _mouseDown;
    private List<Vector2> _points = new List<Vector2>();
    private Curve2D _curve = new Curve2D();
    public override void _Input(InputEvent @event){
        if(@event is InputEventMouseButton button && button.ButtonIndex == (int) ButtonList.Left){
            _mouseDown = @event.IsPressed();
            if(!@event.IsPressed()){
                if (_curve.GetPointCount()>2){ AddChild((Node)PickLine());}
                _curve.ClearPoints();
                _points.Clear();
            }
        }
        if(@event is InputEventKey eventKey){
            if (eventKey.Pressed && eventKey.Scancode == (int)KeyList.R){
                foreach (Node child in GetChildren()){
                    child.QueueFree();
                }
            }
        }
    }
    public override void _Process(float delta){
        Update();
        float t = (float)0.5;
        if(!_mouseDown) return;
        Vector2 mousePos = GetLocalMousePosition();
        if(_points.Count==0 || (_points[_points.Count-1]-mousePos).Length()>5) _points.Add(mousePos);
        if(_curve.GetBakedLength() == 0) _curve.AddPoint(mousePos);
        if(_points.Count==3){
            float d1 = (_points[1]-_points[0]).Length();
            float d2 = (_points[1]-_points[2]).Length();
            _curve.AddPoint(_points[1], -t * d1 / (d1 + d2) * (_points[2]-_points[0]),
                t * d2 / (d1 + d2) * (_points[2]-_points[0]));
            _points.RemoveAt(0);
        }
    }
    private IDrawnLine PickLine(){
        Type[] lineTypes = new Type[]{typeof(DrawnWarding), typeof(DrawnForbiddance), typeof(DrawnVigor)};
        float[] errorThresholds = new float[]{(float) 0.01, (float) 0.0175, (float) 0.4};
        for(int i = 0; i<lineTypes.Length; i++){
            IDrawnLine line = (IDrawnLine) Activator.CreateInstance(lineTypes[i]);
            float error = line.Detect(_curve);
            if(error < errorThresholds[i]){
                ((Label)GetParent().GetNode("Label")).Text = $"{lineTypes[i].Name} error: {error}";
                return line;
            }
        }
        return new Invalid();
    }
    public override void _Draw(){
        if(_curve.GetBakedPoints().Length>2) DrawPolyline(_curve.GetBakedPoints(), Singleton.ChalkColor, 5);
    }
}
//Interface for Drawn Lines
public interface IDrawnLine
{
    float Detect(Curve2D curve);
}
//Types of Drawn Line Types
public class DrawnWarding : Warding, IDrawnLine{
    private float _error = 0;
    public float Detect(Curve2D curve){
        Vector2[] globPts = curve.GetBakedPoints();
        //Setting position
        Position = Vector2.Zero;
        foreach (Vector2 point in globPts){
            Position += point;
        }
        Position /= globPts.Length;
        //Generating Localized Points
        _input = new Vector2[globPts.Length];
        for(int i = 0; i<globPts.Length; i++){
            _input[i]=globPts[i]-Position;
        }
        //Finding radius
        foreach (Vector2 r in _input){
            _radius += r.Length();
        }
        _radius /= _input.Length;

        //Stopping multiple layering
        if(curve.GetBakedLength()/_radius > 3 * Math.PI) return Int32.MaxValue;
        
        //Finding error
        foreach (Vector2 r in _input){
            _error += (float) Math.Pow(r.Length()-_radius, 2);
        }
        _error /= (float) Math.Pow(_radius, 2) * _input.Length;
        _damageFactor = (float) 0.9 + 20 * _error;
        return _error;
    }
    public override void _Draw(){
        base._Draw();
    }
}
public class DrawnForbiddance : Forbiddance, IDrawnLine{
    private float _error;
    public float Detect(Curve2D curve){
        Vector2[] globPts = curve.GetBakedPoints();
        //Setting position
        Position = Vector2.Zero;
        foreach (Vector2 point in globPts){
            Position += point;
        }
        Position /= globPts.Length;
        //Generating Localized Points
        _input = new Vector2[globPts.Length];
        for(int i = 0; i<globPts.Length; i++){
            _input[i]=globPts[i]-Position;
        }
        _dir = LSRL.GetDir(_input);
        foreach (Vector2 r in _input){
            _error += (float) Math.Pow(_dir.Cross(r) , 2);
        }
        _error /= _input.Length * Math.Abs((_dir.Dot(_input[0]) - _dir.Dot(_input[_input.Length-1])));
        _damageFactor = (float) (0.9 * 0.2 / .0175 * _error); 
        return _error;
    }
    public override void _Draw(){
        base._Draw();
    }
}
public class DrawnVigor : Vigor, IDrawnLine{
    private Vector2[] _input;
    private Vector2[] _ideal;
    private List<Vector2> _extrema = new List<Vector2>();
    private float _error = 0;
    private Timer _timer = new Timer();
    public float Detect(Curve2D curve){
        Vector2[] globPts = curve.GetBakedPoints();
        //Setting position
        Position = Vector2.Zero;
        foreach (Vector2 point in globPts){
            Position += point;
        }
        Position /= globPts.Length;
        //Generating localized points
        _input = new Vector2[globPts.Length];
        for(int i = 0; i<globPts.Length; i++){
            _input[i] = globPts[i]-Position;
        }
        //Getting direction
        _dir = LSRL.GetDir(_input);
        float dtheta = (float) 0.08;
        Axis current = new Axis(_dir, _input);
        for (int i = 0; i<10; i++){
            Axis up = new Axis(_dir.Rotated(dtheta), _input);
            Axis down = new Axis(_dir.Rotated(-dtheta), _input);
            dtheta /= 2;
            _dir = _dir.Rotated((up.Amp<down.Amp) ? dtheta : -dtheta);
        }
        if (_input[0].Rotated(-_dir.Angle()).x<0) _dir = -_dir;
        Rotation = _dir.Angle();
        for(int i = 0; i<_input.Length; i++){
            _input[i]=_input[i].Rotated(-_dir.Angle());
        }
        //Guessing peaks and troughs and setting inital x value
        List<Vector2> potential = new List<Vector2>();
        List<Vector2> crestsTroughs = new List<Vector2>();
        Vector2 origin = new Vector2(Int32.MaxValue,0);
        for(int i = 3; i<_input.Length-4; i++){
            float left = 0;
            float right =0;
            for (int n = 1; n<=3; n++){
                left += _input[i-n].y;
                right += _input[i+n].y;
            }
            left/=3;
            right/=3;
            if (left < _input[i].y == right < _input[i].y){
                potential.Add(_input[i]);
            }
            else if (potential.Count>0){
                Vector2 extremum = potential[potential.Count/2];
                if (Math.Abs(extremum.x)<Math.Abs(origin.x)) origin = extremum;
                _extrema.Add(extremum);
                potential.Clear();
            }
        }
        //Avoiding errors
        if (_extrema.Count==0) return Int32.MaxValue;
        //Guessing amplitude
        _amp = new Axis(Vector2.Right, _input).Amp/2 * ((origin.y>0) ? 1: -1);
        //Guessing omega
        _omega = (float)Math.PI*(_extrema.Count-1)/((_extrema[0].x-_extrema[_extrema.Count-1].x));
        //2 period check
        if (4*Math.PI/_omega > _input[0].x-_input[_input.Length-1].x) return Int32.MaxValue;
        //Guessing phase (note to self: for cosine)
        _phase = _omega*origin.x;
        //Generating wave points and error calculation
        _ideal = new Vector2[_input.Length];
        for (int i = 0; i<_input.Length; i++){
            Vector2 newPoint = GetWavePoint(_input[i].x);
            _error += (float) Math.Pow(newPoint.y-_input[i].y, 2);
            _ideal[i] = newPoint;
        }
        _error/= _amp * _amp * _input.Length;
        _damageFactor = (float) (0.5 + Math.Pow(3, - Math.Abs(300 * _omega / _amp)));
        _pushFactor = (float) (0.5 + Math.Pow(3, - Math.Abs(_amp / _omega / 300)));
        GD.Print("Push factor: ", _pushFactor, "Damage Factor: ", _damageFactor);
        return _error;
    }
    private class Axis{
        private Vector2 _dir;
        private float _amp;
        public Axis(Vector2 dir, Vector2[] points){
            _dir = dir;
            float max = points[0].Cross(_dir);
            float min = max;
            float temp;
            foreach (Vector2 point in points){
                temp = point.Cross(_dir);
                max = Math.Max(max, temp);
                min = Math.Min(min, temp);
            }
            _amp = max-min;
        }
        public float Amp{
            get => _amp;
        }
        public Vector2 Dir{
            get => _dir;
        }
    }
    public override void _Ready(){
        _visible = new Vector2[_input.Length];
        _timer.OneShot = true;
        _timer.Autostart = true;
        _timer.WaitTime = (float) 0.2;
        _timer.Connect("timeout", this, nameof(_on_Timer_timeout));
        AddChild(_timer);
    }
    protected override void Form(float delta){
        for(int i = 0; i < _input.Length; i++){
            _visible[i] = _input[i] * _timer.TimeLeft*5 + _ideal[i] * ((float) 1 - _timer.TimeLeft*5);
        }
    }
    public void _on_Timer_timeout(){
        MakeTip();
        _unformed = false;
    }
}
public class Invalid : Node, IDrawnLine{
    private float _error;
    public override void _Ready() {
        QueueFree();
    }
    public float Detect(Curve2D curve){ return 0;}
}
/* Helper class for drawn lines of forbiddance and lines of vigor
   Gets the least squares regression line but residuals are perpendicular to line*/
public static class LSRL{
    public static Vector2 GetDir(Vector2[] input){
    float dx = 0;
    float dy = 0;
    foreach (Vector2 r in input){
        dx += r.x;
        dy += r.y;
    }
    if (dy == 0) return Vector2.Right;
    if (dx == 0) return Vector2.Down;
    double a = 0;
    double b = 0;
    foreach (Vector2 r in input){
        a += Math.Pow(r.x, 2) - Math.Pow(r.y, 2);
        b += r.x * r.y;
    }
    double angle = 0.5 * Math.Atan(2*b/a);
    if (a<0) angle += Math.PI/2;
    return new Vector2((float) Math.Cos(angle), (float) Math.Sin(angle));
    }
}