Vue.js是一套构建用户界面的前端框架。只关注视图层，只关注MVC中的V这一层，注意与界面打交道，来制作界面效果。
MVC  M-Model层，处理数据的增加，读取，查询，删除
      V视图层，前端界面
      C逻辑业务层，controller 控制层：登录，注销。
      
     
 语法
 <body>
    <!-- 将来new的Vue实例，会控制这个元素中的所有内容 -->
    <!-- ******Vue实例所控制的元素区域，就是我们的V -->
    <div id="app">
        <p>{{msg}}</p>
    </div>
    <script>
        // 2.创建一个Vue的实例
        // 当我们导入包之后，在浏览器的内存中，就多了一个Vue构造函数
        // ******注意：我们new出来的这个VM 对象，就是我们MVVM中VM 的调度者
        var vm = new Vue({
            el:'#app',  //表示当前我们new的这个vue实例，要控制页面上的哪个区域
            // *******这里的data就是MVVM中的 M ，保存每个页面的数据的
            data:{      //data属性中，存放的是el中要用到的数据
                msg:'欢迎学习Vue'  //通过Vue提供的指令，很方便就能把数据渲染到页面上
            }
        })
    </script>
</body>



