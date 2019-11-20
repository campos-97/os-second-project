import React, { Component } from 'react';
import { Alert, Button, Image, StyleSheet, View } from 'react-native';

import Img from '../../assets/tictactoe.png';

export default class Login extends Component {
    constructor(props)
    {
      super(props);
    }

  _onPressButton(){
    return fetch('http://192.168.43.104:8888/shapes')
            .then((response) => response.text())
            .then((responseText) => {
                this._askEntry(responseText);
            })
        .catch((error) =>{
        console.error(error);
        });
  }

  _askEntry(shapes) {
    if(shapes == "012"){
        this.props.navigation.navigate('Register', {available: [{code: '0', key: 'X'},
        {code: '1', key: 'O'},
        {code: '2', key: '\u25B3'}]})
    } else if(shapes == "01"){
        this.props.navigation.navigate('Register', {available: [{code: '0', key: 'X'},
        {code: '1', key: 'O'}]})
    } else if(shapes == "02"){
        this.props.navigation.navigate('Register', {available: [{code: '0', key: 'X'},
        {code: '2', key: '\u25B3'}]})
    } else if(shapes == "12"){
        this.props.navigation.navigate('Register', {available: [
        {code: '1', key: 'O'},
        {code: '2', key: '\u25B3'}]})
    }
  }

  _jump(){
    this.props.navigation.navigate('Register')
  }

  render() {
    return (
      <View style={styles.container}>
        <Image 
              source = { Img }
              style={styles.image}
        />
        <View style={styles.buttonContainer}>
        
          <Button
            onPress={this._onPressButton.bind(this)}
            title="Play"
          />
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
   flex: 1,
   justifyContent: 'center',
  },
  buttonContainer: {
    marginLeft: 110,
    width: 150
  },
  image: {
    width: 200,
    height: 200,
    resizeMode: 'contain', 
    marginLeft: 80,
    marginBottom: 80
  }
});