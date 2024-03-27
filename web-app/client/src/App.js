
import { ChakraProvider } from '@chakra-ui/react';
import Home from './Home';

export default function App() {
    return (
        <ChakraProvider>
            <Home />
        </ChakraProvider>
    )
}
