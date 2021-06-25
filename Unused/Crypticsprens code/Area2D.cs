using Godot;

public class Area2D : Area2D{
    public void _on_Area2D_body_exited(Node _body){
        if (_body.GetParent() is DrawnVigor){
            _body.GetParent().QueueFree();
        }
        if (_body is Warding){
            _body.QueueFree();
        }
    }
}

