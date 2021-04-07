const Node = require('../nec.js')

var assert = require('assert');

describe('creating new node', function() {

    context('new node and check value', function() {
      it('should return 1', function() {
        root = new Node(1);
        assert.strictEqual(root.get_value(),1)

      })
    })
    context('new node and children and right neigbour is null',function(){
        it('should return null', function() {
            root = new Node(1);
            assert.strictEqual(root.get_right_neightbour(),null)
            assert.strictEqual(root.get_children_list().length,0)
          })
    })

    context('set right neighour',function(){
        it('should return right neighbour', function() {
            root = new Node(1);
            neighbor = new Node(2);
            root.set_right(neighbor)
            test=root.get_right_neightbour();

            assert.strictEqual(test.get_right_neightbour(),null)
            assert.strictEqual(test.get_children_list().length,0)
            assert.strictEqual(test.get_value(),2);
          })
    })

    context('adding children to root',function(){
        it('should be able to traverse children node', function() {
            root = new Node(1);
            child1= new Node(2);
            child2=new Node(3);

            root.set_right(neighbor)
            root.add_children(child1);
            root.add_children(child2);
            list=root.get_children_list();
            test0=list[0]
            test1=list[1]


            assert.strictEqual(test0.get_right_neightbour(),test1)
            assert.strictEqual(test0.get_children_list().length,0)
            assert.strictEqual(test0.get_value(),2);

            assert.strictEqual(test1.get_children_list().length,0)
            assert.strictEqual(test1.get_value(),3);
            assert.strictEqual(test1.get_right_neightbour(),null);
          })
    })
});
    

