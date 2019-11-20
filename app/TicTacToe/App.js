import { createAppContainer } from 'react-navigation';
import { createStackNavigator } from 'react-navigation-stack';

import Login from './screens/enter'
import Size from './screens/size'
import Shape from './screens/symbol'
import Game from './screens/game'

const RootStack = createStackNavigator({ Main: Login, Register: Shape, Grid: Size, Play: Game });
const AppContainer = createAppContainer(RootStack);

// Now AppContainer is the main component for React to render
export default AppContainer;
