import React, { Component } from 'react';
import { Alert, StyleSheet, FlatList, Text, View, Platform } from 'react-native';

export default class Symbol extends Component {
    constructor(props)
    {
      super(props);
    
      this.state = { GridViewItems: this.props.navigation.getParam('available', 'defaultValue')}
    }

    _login(res, code){
      if(res == 'go'){
        this.props.navigation.navigate('Grid', {shapeCode: code, opCode: ''})
      } else{
        let board = [
          {text: '', key: '00'},
          {text: '', key: '01'},
          {text: '', key: '02'},
          {text: '', key: '10'},
          {text: '', key: '11'},
          {text: '', key: '12'},
          {text: '', key: '20'},
          {text: '', key: '21'},
          {text: '', key: '22'}
        ]
        for(let i = 0; i < 9; i++){
          if(board[i]['key'] == res.slice(1)){
            if(res[0] == 0){
              board[i]['text'] = 'X'
            } else if(res[0] == 1){
              board[i]['text'] = 'O'
            } else{
              board[i]['text'] = '\u25B3'
            }
          }
        }
        this.props.navigation.navigate('Play', {shapeCode: code, opCode: res[0], gameBoard: board})
      }
    }

    GetGridViewItem (code) {
  
      return fetch('http://192.168.43.104:8888/login'+code)
            .then((response) => response.text())
            .then((responseText) => {
                this._login(responseText, code);
            })
        .catch((error) =>{
        console.error(error);
        });
    }

    render() {
        return (
      
     <View style={styles.MainContainer}>
      
           <FlatList
           
              data={ this.state.GridViewItems }
      
              renderItem={({item}) =><View style={styles.GridViewBlockStyle}>
      
                 <Text style={styles.GridViewInsideTextItemStyle} onPress={this.GetGridViewItem.bind(this, item.code)} > {item.key} </Text>
                 
                 </View>}
      
              numColumns={1}
      
             />
        
        
     </View>
                
        );
      }
  
}

const styles = StyleSheet.create({
 
    MainContainer :{
     
    justifyContent: 'center',
    flex:1,
    margin: 10,
    paddingTop: (Platform.OS) === 'ios' ? 20 : 70
     
    },
     
    GridViewBlockStyle: {
     
      justifyContent: 'center',
      flex:1,
      alignItems: 'center',
      height: 100,
      margin: 5,
      backgroundColor: '#00BCD4'
     
    }
    ,
     
    GridViewInsideTextItemStyle: {
     
       color: '#fff',
       padding: 10,
       fontSize: 50,
       justifyContent: 'center',
       
     },
     
    });
