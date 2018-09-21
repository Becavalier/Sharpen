import { h, render, Component, ComponentProps, FunctionalComponent, AnyComponent } from "../../src/preact";

interface DummyProps {
	initialInput: string;
}

interface DummyState {
	input: string;
}

class DummyComponent extends Component<DummyProps, DummyState> {
	constructor(props: DummyProps) {
		super(props);
		this.state = {
			input: `x${this.props}x`
		}
	}

	private setRef = (el: AnyComponent<any>) => {
		console.log(el);
	}

	render({ initialInput }: DummyProps, { input }: DummyState) {
		return (
			<div>
				<DummerComponent initialInput={initialInput} input={input} />
				{/* Can specify all Preact attributes on a typed FunctionalComponent */}
				<ComponentWithChildren initialInput={initialInput} input={input} key="1" ref={this.setRef} />
			</div>
		);
	}
}

interface DummerComponentProps extends DummyProps, DummyState {

}

function DummerComponent({ input, initialInput }: DummerComponentProps) {
	return <div>Input: {input}, initial: {initialInput}</div>;
}

render(h(DummerComponent, { initialInput: "The input", input: "New input", key: "1"}), document);

// Accessing children
const ComponentWithChildren: FunctionalComponent<DummerComponentProps> = (
	{ input, initialInput, children }
) => {
	return <div>
		<span>{initialInput}</span>
		<span>{input}</span>
		<span>{children}</span>
	</div>
}

const UseOfComponentWithChildren = () => {
	return (
		<ComponentWithChildren initialInput="initial" input="input">
			<span>child 1</span>
			<span>child 2</span>
		</ComponentWithChildren>
	);
}


// using ref and or jsx
class ComponentUsingRef extends Component<any, any> {
	private array: string[];
	private refs: Element[] = [];

	constructor() {
		super();
		this.array = ["1", "2"];
	}

	render() {
		this.refs = [];
		return <div jsx>
			{this.array.map(el =>
				<span ref={this.setRef}>{el}</span>
			)}

			{/* Can specify Preact attributes on a component */}
			<DummyComponent initialInput="1" key="1" ref={this.setRef} />
		</div>
	}

	private setRef = (el: Element) => {
		this.refs.push(el);
	}
}

// using lifecycles
class ComponentWithLifecycle extends Component<DummyProps, DummyState> {

	render() {
		return <div>Hi</div>
	}

	componentWillMount() {
		console.log("componentWillMount");
	}

	componentDidMount() {
		console.log("componentDidMount");
	}

	componentWillUnmount() {
		console.log("componentWillUnmount");
	}

	componentWillReceiveProps(nextProps: DummyProps, nextCtx: any) {
		const { initialInput } = nextProps;
		console.log("componentWillReceiveProps", initialInput, nextCtx);
	}

	shouldComponentUpdate(nextProps: DummyProps, nextState: DummyState, nextContext: any) {
		return false;
	}

	componentWillUpdate(nextProps: DummyProps, nextState: DummyState, nextContext: any) {
		console.log("componentWillUpdate", nextProps, nextState, nextContext);
	}

	componentDidUpdate(previousProps: DummyProps, previousState: DummyState, previousContext: any) {
		console.log("componentDidUpdate", previousProps, previousState, previousContext);
	}
}

// Default props: JSX.LibraryManagedAttributes

class DefaultProps extends Component<{text: string, bool: boolean}> {
	static defaultProps = {
		text: "hello"
	};

	render() {
		return <div>{this.props.text}</div>;
	}
}

const d1 = <DefaultProps bool={false} text="foo" />;
const d2 = <DefaultProps bool={false} />;

class DefaultPropsWithUnion extends Component<
	{ default: boolean } & (
		| {
				type: "string";
				str: string;
		  }
		| {
				type: "number";
				num: number;
		  })
> {
	static defaultProps = {
		default: true
	};

	render() {
		return <div />;
	}
}

const d3 = <DefaultPropsWithUnion type="string" str={"foo"} />;
const d4 = <DefaultPropsWithUnion type="number" num={0xf00} />;
const d5 = <DefaultPropsWithUnion type="string" str={"foo"} default={false} />;
const d6 = <DefaultPropsWithUnion type="number" num={0xf00} default={false} />;

class DefaultUnion extends Component<
	| {
			type: "number";
			num: number;
	  }
	| {
			type: "string";
			str: string;
	  }
> {
	static defaultProps = {
		type: "number",
		num: 1
	};

	render() {
		return <div />;
	}
}

const d7 = <DefaultUnion />;
const d8 = <DefaultUnion num={1} />;
const d9 = <DefaultUnion type="number" />;
const d10 = <DefaultUnion type="string" str="foo" />;
