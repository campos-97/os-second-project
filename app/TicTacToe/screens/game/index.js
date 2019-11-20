import React, { Component } from 'react';
import { StyleSheet, FlatList, Text, View, Platform, Alert } from 'react-native';

export default class Game extends Component {
    constructor(props)
    {
      super(props);
    
      this.state = { GridViewItems: this.props.navigation.getParam('gameBoard', 'defaultValue'),
      shapeCode: this.props.navigation.getParam('shapeCode', 'defaultValue'),
      opCode: this.props.navigation.getParam('opCode', 'defaultValue')}
    }

    _getMove(move){
      if(this.state.opCode == ''){
        this.setState({
 
          opCode: move[0]
       
        })
      }

      if(move == 'win'){
        Alert.alert("You win!", [
          {text: 'OK', onPress: () => this.props.navigation.navigate('Main')},
        ])
      } else if(move == 'lose'){
        Alert.alert("Sorry, you lose")/*, [
          {text: 'OK', onPress: () => this.props.navigation.navigate('Main')},
        ])*/
      } else if(move == 'draw'){
        Alert.alert("It's a draw!")/*, [
          {text: 'OK', onPress: () => this.props.navigation.navigate('Main')},
        ])*/
      } else{
        let arr = this.state.GridViewItems;

        for(let i = 0; i < 9; i++){
          if(arr[i]['key'] == move.slice(1)){
            if(move[0] == '0'){
              arr[i]['text'] = 'X'
            } else if(move[0] == '1'){
              arr[i]['text'] = 'O'
            } else if(move[0] == '2'){
              arr[i]['text'] = '\u25B3'
            }
          }
        }

        this.setState({
  
          GridViewItems: arr
      
        })
      }

      
    }

    GetGridViewItem (item) {

      let arr = this.state.GridViewItems;

        for(let i = 0; i < 9; i++){
          if(arr[i]['key'] == item && arr[i]['text'] == ''){
            if(this.state.shapeCode == '0'){
              arr[i]['text'] = 'X'
            } else if(this.state.shapeCode == '1'){
              arr[i]['text'] = 'O'
            } else if(move[0] == '2'){
              arr[i]['text'] = '\u25B3'
            }

            this.setState({
 
              GridViewItems: arr
           
            })
    
            return fetch('http://192.168.43.104:8888/play'+this.state.shapeCode+item)
                  .then((response) => response.text())
                  .then((responseText) => {
                      this._getMove(responseText);
                  })
              .catch((error) =>{
              console.error(error);
              });
          }
          
        }   
    }

    render() {
        return (
      
     <View style={styles.MainContainer}>
      
           <FlatList
           
              data={ this.state.GridViewItems }

              extraData={ this.state }
      
              renderItem={({item}) =><View style={styles.GridViewBlockStyle}>
      
                 <Text style={styles.GridViewInsideTextItemStyle} onPress={this.GetGridViewItem.bind(this, item.key)} > {item.text} </Text>
                 
                 </View>}
      
              numColumns={3}
      
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
      backgroundColor: '#00BCD4',
     
    }
    ,
     
    GridViewInsideTextItemStyle: {
     
       color: '#fff',
       padding: 10,
       fontSize: 50,
       justifyContent: 'center',
       
     },
     
    });
