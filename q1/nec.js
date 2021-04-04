class Node{
    constructor ( value)
    {
        this.children =[]
        this.right = null
        this.value=value
    }

    add_children(child)
    {
        if (this.children.length>0)
        {
            //setup siblings on the right at the sametime
            let index=this.children.length-1
            this.children[index].set_right(child)
        }   
        this.children.push(child)
    }

    set_right(right)
    {
        this.right=right
    }

    get_children_list()
    {
        return this.children
    }

    get_value()
    {
        return this.value
    }

    get_right_neightbour()
    {    
       return this.right 
    }
    
}



let root = new Node(1);
//build the root childrens
root.add_children( new Node(2));
root.add_children(new Node(3));
root.add_children(new Node(4));
let root_children = root.get_children_list();
//build the 2nd layer childrens 
root_children[0].add_children(new Node(5));
root_children[0].add_children(new Node(6));
root_children[2].add_children(new Node(7));
let layer_2_val2_children = root_children[0].get_children_list();
let layer_2_val4_children = root_children[2].get_children_list();
if (layer_2_val4_children.length==1)
    layer_2_val2_children[1].set_right(layer_2_val4_children[0]);

//test if tree traversal works
let cont=true;
let next=root;
let layer=0;
while (cont==true && next!=null)
{   
    console.log('layer %d:',layer)
    i=0;
    first_found=null
    do
    {
        
        console.log('val=%d',next.get_value());
        if (next.get_children_list().length>0 && first_found==null)
        {
            first_found=next.get_children_list()[0];
        }
        next=next.get_right_neightbour();    
    }
    while (next!=null);
    layer=layer+1;

    if (first_found==null)
    {
        cont=false;
    }
    else{
        next=first_found;
    }

    

}

